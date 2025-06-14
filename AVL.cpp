#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

int ReadNum(string massege)
{
	int Number;
	cout << massege;
	cin >> Number;

	while (cin.fail())
	{
		// user didn't input a number
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Invalid Number, Enter a valid one:" << endl;
		cin >> Number;
	}

	return Number;
}


bool leapOrNot(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) ;
}

int CheckDaysInMonth(int year,int month)
{
	if (month < 1 || month>12)
	{
		return 0;
	}
	int arr[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	
	return month == 2 ? leapOrNot(year) ? 29 : 28 :arr[month-1];
		
}


void printDate(int year, int Month, int day)
{
	cout << day << "/" << Month << "/" << year << endl;
}

int calWhatDay(int year, int Month, int day=1)
{
	int a = (14 - Month) / 12;
	int y = year - a;
	int m = Month + 12 * a - 2;
	int d = (day + y + y / 4 - y / 100 + y / 400 + (31 * m) / 12) % 7;

	return d;

	/*string arrDay[] = { "Sunday","Monday","Tuesday","wednesday","Thursday","Friday","Saturday" };

	cout << "Day Order: " << d << endl;
	cout << "Day Name: " << arrDay[d];*/
}

string MonthName(int Month) {
	string arrMonth[] = { "Jan", "Feb", "Mar", "Apr", "May", "June",
						 "July", "Aug", "Sep", "Oct", "Nov", "Dec" };
	return arrMonth[Month - 1];
}

void PrintMainScreen(int Month)
{
	printf("---------------- %s ----------------\n", MonthName(Month).c_str());
	cout << setw(5) << right << "sun"
		<< setw(5) << right << "Mon"
		<< setw(5) << right << "Tue"
		<< setw(5) << right << "Wed"
		<< setw(5) << right << "Thu"
		<< setw(5) << right << "Fri"
		<< setw(6) << right << "Sat\n";
}

int howmanydays(int month, int year)
{
	int arrdays[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	

	return month == 2 ? leapOrNot(year) ? 29 : 28 : arrdays[month - 1];
}

void printdays(int year)
{
	for (int k = 1; k <= 12; k++) {
		PrintMainScreen(k);
		int dayInMonth = howmanydays(k, year);
		int d = calWhatDay(year, k);
		int i;
		for (i = 0; i < d; i++)
			printf("     ");
		for (int j = 1; j <= dayInMonth; j++)
		{
			printf("%5d", j);

			if (++i == 7)
			{
				i = 0;
				printf("\n");
			}
		}
		printf("\n\n");
	}
}


int main()
{

	int year=ReadNum("Input Year: ");
	while(year!=-1){
	//int Month=ReadNum("Input Month: ");
	//int day = ReadNum("Input Day: ");
		
	
	printdays( year);
	system("pause>0");
	system("cls");
	year = ReadNum("Input year: ");

	}
	system("pause>0");
	return 0;
}
