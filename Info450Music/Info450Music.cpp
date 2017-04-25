// Info450Music.cpp
// Scott Roberts

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

class Song{
	string artist;
	string title;
	Song *next;
public:
	Song();
	Song(string, string);
	void CaptureSong();
	void ShowSong();
	int SaveSong(ofstream &outfile);
	friend class SongList;
};

class SongList {
	int numrecords;
	Song *head;
	Song *tail;
public:
	SongList();
	~SongList();
	void GetUserInput();
	void AddNodeToEnd(Song *newnode);
	void ShowSongList();
	int SaveSongList(string filename);
	int ReadSongList(string filename);
	void PlaySongList();
	void DeleteSong(Song *s);
};

Song::Song() {
	artist = "";
	title = "";
	next = NULL;
}

Song::Song(string a, string t) {
	artist = a;
	title = t;
	next = NULL;
}

void Song::ShowSong() {
	cout << "Artist : " << artist << " Title : " << title <<  endl;
}

void Song::CaptureSong()
{
	cout << "Artist? -->";
	getline(cin, artist);
	cout << "Title?  -->";
	getline(cin, title);
	return;
}

int Song::SaveSong(ofstream& outfile)
{
	if (outfile.is_open())
	{
		outfile << artist << "," << title << endl;
		return 0;
	}
	else
		return WRITEERROR;
}

SongList::SongList()
{

	numrecords = 0;
	head = NULL;
	tail = NULL;

}

SongList::~SongList()
{
	Song *ptr = head;
	while (ptr != NULL)
	{
		head = ptr->next;
		delete ptr;
		ptr = head;
	}
}

void SongList::DeleteSong(Song *s) {
	Song *ptr = head;
	if (ptr->title == s->title && ptr->artist == s->artist) {
		head = ptr->next;
		delete ptr;
		return;
	}
	while (ptr != NULL) {
		if (ptr->next->title == s->title && ptr->next->artist == s->artist) {
			ptr->next = ptr->next->next;
			delete ptr;
			return;
		}
	}
}

void SongList::AddNodeToEnd(Song *newnode)
{
	if (head == NULL)
	{
		head = newnode;
		tail = newnode;
	}
	else
	{
		tail->next = newnode;
		tail = newnode;
	}
}


void SongList::GetUserInput()
{
	string answer = "Y";
	cout << "Enter song Y/N?" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		Song *nptr = new Song();
		nptr->CaptureSong();
		AddNodeToEnd(nptr);

		numrecords++;
		cout << "Enter another song Y/N?" << endl;
		getline(cin, answer);
	}
}

void SongList::PlaySongList() {
	Song *ptr = head;
	Song *nptr = NULL;
	string userAnswer;
	string skip;
	int skipAnswer = 0;

	if (head == NULL) {
		return;
	}
	while (ptr != NULL) {
		cout << "Next Song ->" << endl;
		ptr->ShowSong();
		cout << endl << "[D]elete, [S]kip, [P]lay or [Q]uit?  ";
		getline(cin, userAnswer);
		if (userAnswer == "D" || userAnswer == "d") {
			nptr = ptr->next;
			DeleteSong(ptr);
			ptr = nptr;
		}
		if (userAnswer == "S" || userAnswer == "s") {
			cout << "Skip how many?  ";
			getline(cin, skip);
			skipAnswer = stoi(skip);
			for (int i = 0; i < skipAnswer; i++) {
				cout << "Skipping: " << endl;
				ptr->ShowSong();
				ptr = ptr->next;
				if (ptr == NULL) {
					ptr = head;
				}
			}
		}
		if (userAnswer == "P" || userAnswer == "p") {
			cout << "Playing: " << endl;
			ptr->ShowSong();
			ptr = ptr->next;
		}
		if (userAnswer == "Q" || userAnswer == "q") {
			cout << endl << "Quitting: Thanks for listening" << endl;
			return;
		}
		if (ptr == NULL) {
			ptr = head;
		}
	}
}

void SongList::ShowSongList()
{
	Song *ptr = head;
	if (head == NULL)
		return; 

	while (ptr != NULL)
	{
		ptr->ShowSong();
		ptr = ptr->next;
	}
}


int SongList::SaveSongList(string filename)
{
	ofstream output(filename, ios::trunc);
	if (output)
	{
		Song *ptr = head;
		while (ptr != NULL)
		{
			ptr->SaveSong(output);
			ptr = ptr->next;
		}

		output.close();
	}
	else
	{
		return WRITEERROR;
	}
	return 0;
}

int SongList::ReadSongList(string filename)
{
	string a,t;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "File could not be opened for reading" << endl;
		return READERROR;
	}


	while (!infile.eof())
	{

		getline(infile, a, ',');
		if (!a.empty())
		{
			getline(infile, t);
			Song *newnode = new Song(a, t);
			AddNodeToEnd(newnode);

			numrecords++;
		}
	}

	infile.close();
	return 0;
}


int main()
{
	string filename = "C:\\users\\srob300\\desktop\\music.txt";
	SongList sl;
	int error;
	string createList;
	string fileIn;

	cout << "Enter a file? Y/N" << endl;
	getline(cin, fileIn);
	if (fileIn == "Y" || fileIn == "y") {
		cout << "Enter the file." << endl;
		getline(cin, filename);
	}

	error = sl.ReadSongList(filename);
	
	if (error) {
		cout << "Error reading from file.  Create new list? Y/N";
		getline(cin, createList);
		if (createList != "y" && createList != "Y") {
			return 1;
		}
	}
	sl.GetUserInput();
	sl.SaveSongList(filename);
	sl.PlaySongList();
	sl.SaveSongList(filename);
    return 0;
}

