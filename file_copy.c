/*
Titel: file_copy
Beschreibung: Die Funktion file_copy kopiert den Inhalt einer Datei. Dafuer werden
POSIX-Systemcalls (open, read, write, close) sowie ein 20-Byte-Buffer verwendet.
Autor: Patrick Wintner
Datum der letzten Bearbeitung: 27.10.2020
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdbool.h> 

#define BUF_SIZE 20 // Definition der Groesse des Buffers in Byte

// -- Funktion file_copy --
// Parameter:
//	* const char *filename ... Dateiname der zu kopierenden Datei
//	* const char *newfilename ... Dateiname der Kopie
//	* bool err_message_en ... enabelt(true)/disabelt(false) Fehlermeldungen
// Beschreibung: 
// 	file_copy kopiert den Inhalt der Datei filename in die Datei newfilename.
// 	Sollte eine Datei mit den Namen newfilename schon existieren, so wird der Kopiervorgang abgebrochen.
// Rueckgabewert:
//	... wird zur Fehlercodierung verwendet:
//	* 0: kein Fehler aufgetreten
//	* -1/255: Fehler beim Oeffnen der zu kopierenden Datei
//	* -2/254: Fehler beim Oeffnen/Erstellen der Kopie
//	* -3/253: Fehler beim Lesen der zu kopierenden Datei
//	* -4/252: Fehler beim Schreiben in die Kopie
int file_copy(const char *filename, const char *newfilename, bool err_message_en)
{
	int handle_r; // handle fuer die zu kopierende Datei
	int handle_w; // handle fuer Kopie
	void *buf; // Buffer fuer gelesene Daten
	int n_read; // gibt Anzahl gelesener Bytes an

	handle_r = open(filename, O_RDONLY); // O_RDONLY ... nur zum Lesen oeffnen
	if(handle_r == -1) 
	{
		if(err_message_en == true) puts("Fehler beim Oeffnen der zu kopierenden Datei");
		return -1;
	}
	handle_w = open(newfilename, O_CREAT | O_EXCL | O_WRONLY, S_IRWXU); // O_CREAT ... Datei erstellen; O_EXCL ... Fehler, wenn Datei schon existiert;
										// O_WRONLY ... nur zum Schreiben oeffnen; // S_IRWXU ... gibt Eigentuemer Lese-, Schreib- und und Ausfu
										// Ausfuehrungsrechte
	if(handle_w == -1) // Fehlerpruefung
	{
		close(handle_r);
		if(err_message_en == true) puts("Fehler beim Oeffnen/Erstellen der Kopie");
		return -2;
	}
	for(n_read = 20; n_read >= 20;)  // Wenn n_read weniger als 20 => EOF erreicht
	{
		n_read = read(handle_r, buf, BUF_SIZE); // Daten werden in buf eingelesen
		if(n_read == -1)  // Fehlerpruefung
		{
			close(handle_r);
			close(handle_w);
			if(err_message_en == true) puts("Fehler beim Lesen der zu kopierenden Datei");
			return -3;
		}
		if (write(handle_w, buf, n_read) == -1)  // Schreiben der gelesenen Daten in Kopie + Fehlerpruefung
		{
			close(handle_r);
			close(handle_w);
			if(err_message_en == true) puts("Fehler beim Schreiben in die Kopie");
		 	return -4;
		}
	}
	close(handle_r);
	close(handle_w);
	return 0;
}

// -- Funktion clr_str --
// Parameter: *string ... zu saeubernder String
// Beschreibung: clrstr ersetzt \n mit \0 (\n entsteht durch Enter bei Eingabe)
// Rueckgabewert: -
void clr_str(char *string)
{
	for(int i = 0; ;i++)
	{
		if(string[i] == '\n') 
		{
			string[i] = '\0';
			break;
		}
	}
}

int main()
{
	char filename[BUF_SIZE+1]; // 20 Zeichen + \0
	char newfilename[BUF_SIZE+1]; // 20 Zeichen + \0

	puts("Bitte Namen der zu kopierenden Datei eingeben:");
	fgets(filename, BUF_SIZE, stdin);
	clr_str(filename); // um \n durch \0 zu ersetzen
	puts("\nBitte Namen der neuen Datei eigeben:");
	fgets(newfilename, BUF_SIZE, stdin);
	clr_str(newfilename); // um \n durch \0 zu ersetzen

	return file_copy(filename, newfilename, true);
}