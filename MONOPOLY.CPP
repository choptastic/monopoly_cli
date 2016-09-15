#include <stdlib.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <malloc.h>

const int MAXPLAYERS=8;
const int NO=0;
const int YES=1;
const int BANK=200;
const int FREEPARKING=100;
const int UNOWNED=300;
const char* SAVEFILE="monopoly.sav";


char* chance[16];
char* commchest[16];
char* properties[40];


int houses[40];
int prices[40];
int rent[40][6];
int owned[40];
int fund[MAXPLAYERS];
int position[MAXPLAYERS];
int exist[MAXPLAYERS];
int freefund=0;



void setup();
void loadinfo();

void save();
void load();

void help();


int upgradable(int prop);
int isprop(int prop);
int getrent(int prop);
int ismonopoly(int prop);
int thesame(int x, int y, int z);

void advance(int player, int spaces);
void goback(int player,int spaces);
void move(int player, int position, int passgo);
void transfer(int from, int to, int amt);
void changehouses(int prop, int amt);
void propinfo(int prop);
void funds();
void positioninfo(int player);
void createplayer(int player);
void destroy(int player);
void jail(int player);
void buy(int player, int prop, int amt);
void transferprop(int from, int to, int prop);
void restart();
void mortgagetoggle(int prop);
void listprops(int player);
void listunowned();

int interpretcommand(char line[], char &comm, int &arg1, int &arg2, int &arg3);


void main()
{
	randomize();
	loadinfo();
	setup();
	textcolor(7);
	textbackground(0);
	clrscr();
	char comm;
	int arg1,arg2,arg3,error;
	char commandline[30];
	//cin.getline(commandline,30);
	cout << "Whoa! Productions' Monopoly assistant, v0.2\n";
	while(comm!='Q')
	{
		cout << "Command: ";
		cin.getline(commandline,30);
		error=interpretcommand(commandline,comm,arg1,arg2,arg3);
		if(error==-1 && comm!='Q')
			cout << "Invalid Syntax\n";
		else
		{
			switch(comm)
			{

				case('W'):	listprops(arg1);
								break;
				case('U'):	listunowned();
								break;
				case('O'):	mortgagetoggle(arg1);
								break;
				case('L'):	load();
								break;
				case('A'):	advance(arg1, arg2);
								break;
				case('G'):	goback(arg1, arg2);
								break;
				case('M'):  move(arg1,arg2,YES);
								break;
				case('T'):  transfer(arg1,arg2,arg3);
								break;
				case('B'):	buy(arg1,arg2,arg3);
								break;
				case('J'):	jail(arg1);
								break;
				case('R'):	transferprop(arg1,arg2,arg3);
								break;
				case('H'):  changehouses(arg1,arg2);
								break;
				case('I'):  propinfo(arg1);
								break;
				case('F'):  funds();
								break;
				case('K'):	cout << "Free Parking: $" << freefund << endl;
								break;
				case('P'):  positioninfo(arg1);
								break;
				case('C'):	createplayer(arg1);
								break;
				case('D'):	destroy(arg1);
								break;
				case('?'):	help();
								break;
				case('Q'):	cout << "Thank you for choosing Whoa! Productions\n";
								break;
				case('S'):	restart();
								break;
				default:		cout << "Invalid command\n";
			}
		}
		save();
	}
}

int isprop(int prop)
{
	switch(prop)
	{
		case(0):
		case(2):
		case(4):
		case(7):
		case(10):
		case(17):
		case(20):
		case(22):
		case(30):
		case(33):
		case(36):
		case(38):	return NO;
		default :	return YES;
	}
}

int upgradable( int prop)
{
	switch(prop)
	{
		case(1):
		case(3):
		case(6):
		case(8):
		case(9):
		case(11):
		case(13):
		case(14):
		case(16):
		case(18):
		case(19):
		case(21):
		case(23):
		case(24):
		case(26):
		case(27):
		case(29):
		case(31):
		case(32):
		case(34):
		case(37):
		case(39):	return YES;
		default:		return NO;
	}
}

void jail(int player)
{
	if(!exist[player])
		cout << "Player Nonexistant\n";
	else
		move(player,10,NO);
}

void buy(int player, int prop, int amt)
{
	if(!exist[player])
		cout << "Player Nonexistant\n";
	else if(!isprop(prop))
		cout << "Not a Property\n";
	else if(owned[prop]!=UNOWNED)
		cout << "Property already owned\n";
	else if(fund[player]-amt<0)
		cout << "Insufficient funds\n";
	else
	{
		owned[prop]=player;
		transfer(player,BANK,amt);
	}
}


void help()
{
	cout  << "L = load" << endl
			<< "C [player] = create" << endl
			<< "D [player] = destroy" << endl
			<< "A [player] [spaces] = advance" << endl
			<< "M [player] [space] = move" << endl
			<< "G [player] [spaces] = go back" << endl
			<< "J [player] = Jail" << endl
			<< "B [player] [property] [amount] = Buy" << endl
			<< "R [from] [to] [property] = Transfer property" << endl
			<< "H [property] [numhouses] = change amount of houses on [property] to [amount]" << endl
			<< "O [property] = toggle mortgage" << endl
			<< "T [from] [to] [amount] = Transfer funds" << endl
			<< "I [space] = space info" << endl
			<< "P [player] = space info of square [player] is on" << endl
			<< "W [player] = list all properties [player] owns" << endl
			<< "U = List all unowned properties" << endl
			<< "F = Funds of all players" << endl
			<< "K = Free Parking Fund" << endl
			<< "200 = Bank (only in transfers)" << endl
			<< "? = This help screen" << endl;
}



void transferprop(int from, int to, int prop)
{
	if(!isprop(prop))
		cout << "Not a property\n";
	else if(owned[prop]!=from)
		cout << "Property not owned by player " << from << endl;
	else if(houses[prop]!=0)
		cout << "Cannot sell a property that is developed\n";
	else if(!exist[from] || !exist[to])
		cout << "Player nonexistant\n";
	else
		owned[prop]=to;
}

int thesame(int x,int y,int z)
{
	return (x==y && x==z);
}


int getrent(int prop)
{
	int loop,num=0;
	switch(prop)
	{
		case(5):
		case(15):
		case(25):
		case(35):   num=25;
						for(loop=5;loop<=35;loop+=10)
							if(owned[prop]==owned[loop] && prop!=loop && owned[loop]!=UNOWNED)
								num*=2;
						return num;
		case(28):
		case(12):   gotoxy(wherex()-1,wherey());
						cout << "amount thrown times $";
						if(owned[12]==owned[28])
							return 10;
						else
							return 4;
		default:		if(houses[prop]==0)
						{
							if(ismonopoly(prop))
								return rent[prop][0]*2;
							else
								return rent[prop][0];
						}
						else
							return rent[prop][houses[prop]];
	}
}

int ismonopoly(int prop)
{
	int num=0;
	switch(prop)
	{
		case(1):
		case(3):	if(owned[1]==owned[3]) num=1;
					break;
		case(6):
		case(8):
		case(9):	if(thesame(owned[6],owned[8],owned[9])) num=1;
					break;
		case(11):
		case(13):
		case(14):if(thesame(owned[11],owned[13],owned[14])) num=1;
					break;
		case(16):
		case(18):
		case(19):if(thesame(owned[16],owned[18],owned[19])) num=1;
					break;
		case(21):
		case(23):
		case(24):if(thesame(owned[21],owned[23],owned[24])) num=1;
					break;
		case(26):
		case(27):
		case(29):if(thesame(owned[26],owned[27],owned[29])) num=1;
					break;
		case(31):
		case(32):
		case(34):if(thesame(owned[31],owned[32],owned[34])) num=1;
					break;
		case(37):
		case(39):if(owned[37]==owned[39]) num=1;
					break;
	}
	return num;

}

void listprops(int player)
{
	for(int loop=0;loop<40;loop++)
		if(owned[loop]==player && isprop(loop))
			cout << "Space " << loop << ": " << properties[loop] << endl;

}

void listunowned()
{
	listprops(UNOWNED);
}



void mortgagetoggle(int prop)
{
	if(owned[prop]==UNOWNED)
		cout << "Property Unowned\n";
	else if(houses[prop]>0)
		cout << "Cannot mortgage properties that are developed\n";
	else
	{
		if(houses[prop]==0)
		{
			fund[owned[prop]]+=(prices[prop]/2);
			cout << "Mortgaged " << properties[prop] << endl;
			houses[prop]=-1;
		}
		else
		{
			if(fund[owned[prop]]-(prices[prop]/2)<0)
				cout << "Insufficient Funds\n";
			else
			{
				fund[owned[prop]]-=(prices[prop]/2);
				cout << "Unmortgaged " << properties[prop] << endl;
				houses[prop]=0;
			}
		}
	}
}


void advance(int player, int spaces)
{
	if(!exist[player])
		cout << "Player nonexistant\n";
	else if(spaces<2 || spaces>12)
		cout << "Invalid number of Spaces\n";
	else
	{
		position[player]+=spaces;
		if(position[player]>39)
		{
			transfer(BANK,player,200);
			position[player]-=40;
			cout << "Passed GO, player " << player << " credited $200\n";
		}
		positioninfo(player);
		switch(position[player])
		{
			case(7):
			case(22):
			case(36): 	cout << chance[random(16)] << endl;
							break;
			case(2):
			case(17):
			case(33):	cout << commchest[random(16)] << endl;
							break;
			case(30):	move(player,10,NO);
							break;
			case(20):   cout << "Player " << player << " credited $" << freefund << endl;
							transfer(FREEPARKING,player,freefund);
							break;
		}

	}


}

void goback(int player, int spaces)
{
	if(!exist[player])
		cout << "Nonexistant Player";
	else
	{
		position[player]-=spaces;
		if(position[player]<0)
			position[player]+=40;
		positioninfo(player);
	}
}

void move(int player, int pos, int passgo)
{
	if(!exist[player])
		cout << "Player nonexistant\n";
	else
	{
		if(passgo==YES && pos<position[player])
		{
			cout << "Passed GO, player " << player << " credited $200\n";
			transfer(BANK,player,200);
		}
		position[player]=pos;
		propinfo(pos);
	}
}

void transfer(int from, int to, int amt)
{
	if(!exist[from] || !exist[to])
		cout << "Player nonexistant\n";
	else
	{
		if(from==BANK)
			fund[to]+=amt;
		else if(to==BANK)
		{
			if(fund[from]-amt<0)
				cout << "Insufficient funds\n";
			else
				fund[from]-=amt;
		}
		else if(from==FREEPARKING)
		{
			fund[to]+=freefund;
			freefund=0;
		}
		else if(to==FREEPARKING)
		{
			if(fund[from]-amt<0)
				cout << "Innsufficient funds\n";
			else
			{
				fund[from]-=amt;
				freefund+=amt;
			}
		}
		else
		{
			if(fund[from]-amt<0)
				cout << "Insufficient funds\n";
			else
			{
				fund[to]+=amt;
				fund[from]-=amt;
			}
		}
	}
}


void changehouses(int prop, int amt)
{

	if(!upgradable(prop))
		cout << "Property Not Upgradable\n";
	else if(owned[prop]==UNOWNED)
		cout << "Property Unowned\n";
	else if(!ismonopoly(prop))
		cout << "Not a monopoly\n";
	else
	{
		int perhouse=((prop/10)+1)*50;
		if((amt-houses[prop])*perhouse>fund[owned[prop]])
			cout << "Insufficient funds\n";
		else if(amt>5 || amt <0)
			cout << "Houses must be between 0 and 5\n";
		else
		{
			if(amt<houses[prop])
				perhouse/=2;

			fund[owned[prop]]-=((amt-houses[prop])*perhouse);

			houses[prop]=amt;
			propinfo(prop);
		}
	}
}


void propinfo(int prop)
{
	cout 	<< "Property index: " << prop << endl
			<< "Property Name: " << properties[prop] << endl;
	if(isprop(prop))
	{
		if(owned[prop]==UNOWNED)
			cout << "Unowned. . . it costs $" << prices[prop] << endl;
		else
			cout << "Owned by player " << owned[prop] << endl;
	}
	if(isprop(prop) && owned[prop]!=UNOWNED)
	{
		if(houses[prop]==-1)
			cout << "Mortgaged\n";
		else
		{
			if(upgradable(prop))
				cout << "Number of houses (5 for hotel): " << houses[prop] << endl;
			cout << "Rent Price: $";
			cout << getrent(prop) << endl;
		}
	}
}

void funds()
{
	for(int loop=0;loop<MAXPLAYERS;loop++)
		if(exist[loop])
			cout << "Player " << loop << " funds: $" << fund[loop] << endl;
}


void positioninfo(int player)
{
	if(!exist[player])
		cout << "Player nonexistant\n";
	else
		propinfo(position[player]);
}

void createplayer(int player)
{
	if(exist[player])
		cout << "Player " << player << " already exists\n";
	else
	{
		exist[player]=1;
		fund[player]=1500;
	}
}

void destroy(int player)
{
	if(!exist[player])
		cout << "Player " << player << " already nonexistant\n";
	else
	{
		exist[player]=0;
		fund[player]=0;
		for(int loop=0;loop<40;loop++)
			if(owned[loop]==player)
				owned[loop]=UNOWNED;
	}
}








void restart()
{
	char choice;
	cout << "Are you sure you want to restart the current game (Y/N)\n";
	do{
		choice=toupper(getch());
	}while(choice!='Y' && choice!='N');
	if(choice=='Y')
		setup();
	else
		cout << "Restart Cancelled\n";
}



void save()
{
	ofstream out(SAVEFILE);
	for(int loop=0;loop<40;loop++)
		out << houses[loop] << endl
			 << owned[loop] << endl;
	for(loop=0;loop<MAXPLAYERS;loop++)
	{
		out<< fund[loop] << endl
			<< position[loop] << endl
			<< exist[loop] << endl;
	}
	out << freefund << endl;
	out.close();

}


void load()
{
	char choice;
	cout << "Are you sure you want to load and end current game? (Y/N)\n";
	do{
		choice=toupper(getch());
	}while(choice!='Y' && choice!='N');
	if(choice=='Y')
	{

		ifstream in(SAVEFILE);
		for(int loop=0;loop<40;loop++)
			in >> houses[loop] >> owned[loop];
		for(loop=0;loop<MAXPLAYERS;loop++)
			in >> fund[loop] >> position[loop] >> exist[loop];
		in >> freefund;
		in.close();
	}
	else
		cout << "Load Cancelled\n";
}

void setup()
{
	for(int loop=0;loop<MAXPLAYERS;loop++)
	{
		exist[loop]=0;
		fund[loop]=0;
		position[loop]=0;
	}

	for(loop=0;loop<40;loop++)
	{
		houses[loop]=0;
		owned[loop]=UNOWNED;
	}


}

void loadinfo()
{

	int temp;
	for(int loop=0;loop<16;loop++)
	{
		chance[loop]=(char*)malloc(150);
		commchest[loop]=(char*)malloc(150);
	}
	for(loop=0;loop<40;loop++)
		properties[loop]=(char*)malloc(35);
	ifstream ichance,icomm,iprops,iprice,irent;

	ichance.open("chance.txt");
	icomm.open("chest.txt");
	iprops.open("squares.txt");
	iprice.open("price.txt");
	irent.open("rent.txt");
	for(loop=0;loop<16;loop++)
	{
		ichance.getline(chance[loop],150);
		icomm.getline(commchest[loop],150);
	}
	for(loop=0;loop<40;loop++)
		iprops.getline(properties[loop],30);
	for(loop=0;loop<22;loop++)
	{
		irent >> temp;
		for(int i=0;i<6;i++)
			irent >> rent[temp][i];
		iprice >> temp;
		iprice >> prices[temp];
	}
	for(loop=5;loop<40;loop+=10)
		prices[loop]=200;
	prices[12]=150;
	prices[28]=150;
	ichance.close();
	icomm.close();
	iprops.close();
	irent.close();
	iprice.close();
}




int interpretcommand(char line[], char &comm, int &arg1, int &arg2, int &arg3)
{
	int loc=0,startloc,endloc,loop;
	char a1[6],a2[6],a3[6];

	for(loop=0;loop<6;loop++)
		a1[loop]=a2[loop]=a3[loop]='\0';


	for(loop=0;loop<50;loop++)
		line[loop]=toupper(line[loop]);

	loc=0;
	while(line[loc]==' ')
		loc++;

	if((line[loc]>='A' && line[loc]<='Z') || line[loc]=='?')
	{
		comm=line[loc];
		loc++;
	}
	else
		return -1;

	if(line[loc]!=' ' && line[loc]!='\n' && line[loc]!='\0')
		return -1;


	while(line[loc]==' ')
		loc++;

	if(line[loc]!='\n' && line[loc]!='\0')
	{
		startloc=loc;
		while(line[loc]>='0' && line[loc]<='9')
			loc++;
		endloc=loc;
		for(loop=0;loop<endloc-startloc;loop++)
			a1[loop]=line[loop+startloc];
	}


	while(line[loc]==' ')
		loc++;

	if(line[loc]!='\n' && line[loc]!='\0')
	{
		startloc=loc;
		while(line[loc]>='0' && line[loc]<='9')
			loc++;
		endloc=loc;
		for(loop=0;loop<endloc-startloc;loop++)
			a2[loop]=line[loop+startloc];
	}

	while(line[loc]==' ')
		loc++;

	if(line[loc]!='\n' && line[loc]!='\0')
	{
		startloc=loc;
		while(line[loc]>='0' && line[loc]<='9')
			loc++;
		endloc=loc;
		for(loop=0;loop<endloc-startloc;loop++)
			a3[loop]=line[loop+startloc];
	}
	arg1=atoi(a1);
	arg2=atoi(a2);
	arg3=atoi(a3);
	return 1;
}