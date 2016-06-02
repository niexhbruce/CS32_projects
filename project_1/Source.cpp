#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <unordered_map>
using namespace std;

const int N = 16;
const int ADD = 1;
const int COPY = 2;

bool getInt(istream& inf, int& n)
{
	char ch;
	if (!inf.get(ch)  ||  !isascii(ch)  ||  !isdigit(ch))
		return false;
	inf.unget();
	inf >> n;
	return true;
}

bool getCommand(istream& inf, char& cmd, int& length, int& offset)
{
	if (!inf.get(cmd)  ||  (cmd == '\n'  &&  !inf.get(cmd)) )
	{
		cmd = 'x';  // signals end of file
		return true;
	}
	char ch;
	switch (cmd)
	{
	case 'A':
		return getInt(inf, length) && inf.get(ch) && ch == ':';
	case 'C':
		return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
	}
	return false;
}

void createDelta(ifstream& of, ifstream& nf, ofstream& df)
{
	string old_content, new_content, delta_s;
	char c;
	while(of.get(c))
		old_content += c;
	while(nf.get(c))
		new_content += c;
	unordered_map<string,int> string_set;
	for(int i = 0; i < old_content.length(); i++)
	{
		string tmp = old_content.substr(i, N);
		string_set[tmp] = i;
	}
	int j = 0;
	int pre_op = ADD;
	int copy_pos = -1;
	int copy_end = -1;
	string add_s = "";
	while(j < new_content.length())
	{
		string tmp = new_content.substr(j,N);
		if(string_set.find(tmp) != string_set.end())
		{
			if(pre_op == COPY)
			{
				if((copy_end + 1) == string_set[tmp])
					copy_end += N;
				else
				{
					delta_s += "C" + to_string(copy_end - copy_pos + 1) + "," + to_string(copy_pos);
					copy_pos = string_set[tmp];
					copy_end = copy_pos + tmp.length() - 1;
				}
			}
			else if(pre_op == ADD)
			{
				delta_s += "A" + to_string(add_s.length()) + ":" + add_s;
				copy_pos = string_set[tmp];
				copy_end = copy_pos + tmp.length() - 1;
				pre_op = COPY;
				add_s = "";
			}
			j += N;
		}
		else
		{
			if(pre_op == COPY)
			{
				delta_s += "C" + to_string(copy_end - copy_pos + 1) + "," + to_string(copy_pos);
				copy_pos = -1;
				copy_end = -1;
				add_s += new_content[j];
				pre_op = ADD;
			}
			else if(pre_op == ADD)
			{
				add_s += new_content[j];
			}
			j++;
		}
	}
	if(pre_op == COPY)
		delta_s += "C" + to_string(copy_end - copy_pos + 1) + "," + to_string(copy_pos);
	else if(pre_op == ADD)
		delta_s += "A" + to_string(add_s.length()) + ":" + add_s;
	df << delta_s;
}

bool applyDelta(ifstream& of, ifstream& df, ofstream& nf)
{
	char cmd;
	int offset, l;
	string old_c;
	char c;
	while(of.get(c))
		old_c += c;
	while(getCommand(df, cmd, l, offset) && cmd != 'x')
	{
		if(cmd == 'A')
			for(int i = 0; i < l; i++)
			{
				char c;
				c = df.get();
				nf << c;
			}
		if(cmd == 'C')
			nf << old_c.substr(offset, l);
	}
	return true;
}

bool runtest(string oldname, string newname, string deltaname, string newname2)
{
	ifstream oldfile(oldname);
	if (!oldfile)
	{
		cerr << "Cannot open " << oldname << endl;
		return false;
	}
	ifstream newfile(newname);
	if (!newfile)
	{
		cerr << "Cannot open " << newname << endl;
		return false;
	}
	ofstream deltafile(deltaname);
	if (!deltafile)
	{
		cerr << "Cannot create " << deltaname << endl;
		return false;
	}
	createDelta(oldfile, newfile, deltafile);
	deltafile.close();

	oldfile.clear();   // clear the end of file condition
	oldfile.seekg(0);  // reset back to beginning of the file
	ifstream deltafile2(deltaname);
	if (!deltafile2)
	{
		cerr << "Cannot read the " << deltaname << " that was just created!" << endl;
		return false;
	}
	ofstream newfile2(newname2);
	if (!newfile2)
	{
		cerr << "Cannot create " << newname2 << endl;
		return false;
	}
	assert(applyDelta(oldfile, deltafile2, newfile2));
	cout << "You must compare " << newname << " and " << newname2 << endl;
	cout << "If they are not identical, you failed this test." << endl;
	return true;
}

int main()
{
	assert(runtest("warandpeace1.txt", "warandpeace2.txt", "mydeltafile.txt", "mynewfile2.txt"));
}