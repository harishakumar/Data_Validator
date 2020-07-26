#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <stdio.h>

#include "../Binary_Tree/Binary_Tree.h"
#include "../Merkle_Tree/Merkle_Tree.h"

using namespace std;

char **string_process(string input_string, int);
int packet_size;

int main()
{
	cout << "Merkle_Tree implementation";
	cout << " 1 - Send a message" << endl << "2 - Check for validation" << endl;
	int input;
	unsigned long int master_hash;
	char char_input, tamper_flag;
	string message, tampered_message;
	fstream master_hash_file, msg_packets_file;
	cin >> input ;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	int split_size;
	unsigned long long int *hash_array;
	cout << "Enter split size : ";
	cin >> split_size;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	if(split_size == 2)
		split_size = 2;
	else if(split_size % 4 != 0)
		split_size = split_size - (split_size%4);

	if (input == 1)
	{
		Merkle_Tree T(1);
		cout << "Enter your message : ";
		getline(cin, message, '\n');
		char **msg_packets = string_process(message, split_size);
		hash_array = T.create_merkle_tree(msg_packets, split_size);
		cout << endl;
		T.level_order_traversal( T.get_root() );
		master_hash = T.get_master_root();
		cout << endl << "The Master hash is : " << master_hash << endl;
		cout << "Do you want to tamper the msg (Y/n) : " << endl;
		cin >> tamper_flag;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if(tamper_flag == 'n')
		{
			master_hash_file.open("master_hash.txt");
			master_hash_file << master_hash;
			master_hash_file.close();
			msg_packets_file.open("msg_packets.txt");
			for(int i=0; i<split_size; i++)
			{
				string input_part(msg_packets[i], packet_size);
				msg_packets_file << input_part << endl;
			}
			msg_packets_file.close();
		}
		else if(tamper_flag == 'Y' || tamper_flag == 'y')
		{
			cout << "Enter your Tampered message: ";
			getline(cin, tampered_message, '\n');
			char** msg_packets_tampered = string_process(tampered_message, split_size);
			msg_packets_file.open("msg_packets.txt");
			for(int i=0; i<split_size; i++)
			{
				string input_part(msg_packets_tampered[i], packet_size);
				msg_packets_file << input_part << endl;
			}
			msg_packets_file.close();
		}
	}
	cout << "Do you want to check for validation (Y/n): ";
	cin >> char_input;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	ifstream receiver_packets_file;
	if(char_input == 'Y' || char_input == 'y')
	{
		string input_from_file;
		char** receiver_message = new char* [split_size];
		receiver_packets_file.open("msg_packets.txt");
		for(int i=0; i<split_size; i++)
		{
			receiver_packets_file >> input_from_file;
			cout << input_from_file << endl;
			char *cstr = new char[input_from_file.length() + 1];
			strcpy(cstr, input_from_file.c_str());
			receiver_message[i] = cstr;
		}
		receiver_packets_file.close();
		Merkle_Tree verification(1);
		unsigned long long int *hash_array2 = verification.create_merkle_tree(receiver_message, split_size);
		unsigned long long int chk_hash = verification.get_master_root();
		if(chk_hash == master_hash)
		{
			cout << endl << "Congrats, The message is not Tampered" << endl<<endl;
			cout << "The Received Message is: ";
			for(int i=0; i<split_size; i++)
			{
				for(int j=0; j<packet_size; j++)
				{
					if(receiver_message[i][j] != '$')
						cout << receiver_message[i][j];
				}
			}
			cout << endl;
		}
		else
		{
			cout << endl << "Oops, The message has been Tampered" << endl << endl;
			cout << "The Received message is: ";
			for(int i=0; i<split_size; i++)
			{
				for(int j=0; j<packet_size; j++)
				{
					if(receiver_message[i][j] != '$')
						cout << receiver_message[i][j];
				}
			}
			cout << endl;
			for(int i=(2*split_size); i>= 1; i--)
			{
				if(hash_array2[i] != hash_array[i])
				{
					cout << "Tampered Nodes: " << i << endl;
				}
			}
			cout << endl;
		}
	}
	else
	{
		cout << "The Program is Terminated" << endl;
	}
	return 0;
}

char** string_process(string input_string, int split_size)
{
	int length = input_string.length();
	char** msg_packets = new char* [split_size];
	if(length%split_size == 0)
	{
		packet_size = ((input_string.length())/ split_size);
		for(int i=0; i<split_size; i++)
		{
			char *temp= new char [ packet_size + 1];
			for(int j=0; j<packet_size; j++)
			{
				temp[j] = input_string[j];
			}
			temp[packet_size] = '\0';
			input_string.erase(input_string.begin(), input_string.begin()+packet_size);
			msg_packets[i] = temp;
		}
	}
	else
	{
		int parity_adder = split_size - (length % split_size);
		for(int i=1; i<=parity_adder; i++)
		{
			input_string += "$";
		}
		packet_size = (input_string.length() / split_size);
		for(int i=0; i<split_size; i++)
		{
			char *temp = new char [ packet_size + 1];
			for(int j=0;j<packet_size; i++)
			{
				temp[j] = input_string[j];
			}
			temp[packet_size] = '\0';
			input_string.erase(input_string.begin(), input_string.begin() + packet_size);
			msg_packets[i] = temp;
		}
	}
	return msg_packets;
}