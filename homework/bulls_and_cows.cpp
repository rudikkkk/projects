#include <std_lib_facilities.h>
#include <string_view>

constexpr int LENGTH = 4;

auto generate_number ()
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1000, 9999);
  return dis(gen);
}

struct num
{
  vector<int> number = vector<int>(LENGTH);

  num (int a) { update(a); }

  num (vector<int> a) { number = a; }

  num (string a) { update(a); }

  void update (int a)
  {
    for (int i{0}; i < LENGTH; ++i)
    {
      number[i] = a / static_cast<int>(pow(10, LENGTH - 1 - i)) % 10;
    }
  }

  void update (string a)
  {
    for (int i{0}; i < LENGTH; ++i)
    {
      number[i] = a[i] - '0';
    }
  }

  bool validate ()
  {
    for (int i{0}; i < LENGTH; ++i)
    {
      for (int j{i + 1}; j < LENGTH; ++j)
      {
        if (number[i] == number[j])
        {
          return false;
        }
      }
    }
    return true;
  }

  string toString ()
  {
    string result = "";
    for (int i{0}; i < LENGTH; ++i)
    {
      result += to_string(number[i]);
    }
    return result;
  }

  bool operator== (const num& other)
  {
    for (int i{0}; i < LENGTH; ++i)
    {
      if (number[i] != other.number[i])
      {
        return false;
      }
    }
    return true;
  }
};

struct solver

{
  vector<num> falseNumbers;
  num currNumber{generate_number()};

  string do_prediction ()
  {
    while (find(falseNumbers.begin(), falseNumbers.end(), currNumber) !=
               falseNumbers.end() ||
           !currNumber.validate())
    {
      currNumber.update(generate_number());
    }

    falseNumbers.push_back(currNumber);
    return currNumber.toString();
  }
};

int count (const string& digits, char d)
{
  int k{};

  for (int i = 0; i < LENGTH; ++i)
    if (digits[i] == d)
      ++k;

  return k;
}

void countBullsAndCows (vector<int>& bullsCows, num refNum, num compNum)
{
  bullsCows[0] = 0;
  bullsCows[1] = 0;

  for (int i{0}; i < LENGTH; ++i)
  {
    for (int j{0}; j < LENGTH; ++j)
    {
      if (refNum.number[i] == compNum.number[j])
      {
        if (i == j)
        {
          ++bullsCows[0];
        }
        else
        {
          ++bullsCows[1];
        }
      }
    }
  }
}

num user_guess ()
{
  string str = "";
  cout << "guess the number: ";
  if (cin >> str)
  {
    if (str.size() > LENGTH)
    {
      error("the number contains too many characters");
    }
    else if (str.size() < LENGTH)
    {
      error("the number contains too few characters");
    }
    else
    {
      for (int i{0}; i < LENGTH; ++i)
      {
        if (str[i] < '0' || '9' < str[i])
        {
          error("the number contains not a digit");
        }
      }
      num number{str};
      if (number.validate())
      {
        return number;
      }
    }
  }
  error("invalid input");
}

void start_game ()
{
  solver bot;
  num botNumber{0000};
  do
  {
    num botNumber{generate_number()};
  }
  while (!botNumber.validate());

  vector<int> bullsCows(2);

  do
  {
    try
    {
      num uguess = user_guess();
      countBullsAndCows(bullsCows, botNumber, uguess);
    }
    catch (const exception& e)
    {
      if (std::string_view(e.what()) == "the number contains not a digit" ||
          std::string_view(e.what()) ==
              "digits of the number are not unique" ||
          std::string_view(e.what()) ==
              "the number contains too many characters" ||
          std::string_view(e.what()) ==
              "the number contains too few characters")
      {
        cerr << e.what() << endl;
        continue;
      }
      else
      {
        error(e.what());
      }
    }
    cout << bullsCows[0] << " bull(s) and " << bullsCows[1] << " cow(s)"
         << endl;

    if (bullsCows[0] != 4)
    {
      cout << "My move: " << bot.do_prediction() << "? (y/n): ";
      while (true)
      {
        string t;
        cin >> t;

        if (t == "y")
        {
          std::cout << "bot win, ha-ha" << std::endl;
          return;
        }
        if (t == "n")
        {
          break;
        }
        std::cout << "invalid answer. (y/n): ";
      }
      vector<int> botBullsCows{0, 0};
      cout << "Bulls: ";
      cin >> botBullsCows[0];
      cout << "Cows: ";
      cin >> botBullsCows[1];
    }
    else
    {
      cout << "you win!" << endl;
    }
  }
  while (bullsCows[0] != 4);
}

int main ()
try
{
  cout << "=====================================================\n"
       << "             The game \"Bulls and Cows\"\n"
       << "=====================================================\n"
       << "     Computer sets a number of 4 unique digits.\n"
       << "                  Try to guess it!\n"
       << "=====================================================\n"
       << "         Bull - digit in the correct position\n"
       << "         Cow - right digit in the wrong place.\n"
       << "=====================================================\n\n";
  char answer = 'n';
  do
  {
    start_game();
    cout << "play again? (y/n) ";
    cin >> answer;
  }
  while (answer == 'y' || answer == 'Y');
}
catch (exception& e)
{
  cerr << e.what() << endl;
  return 1;
}
catch (...)
{
  cerr << "Oops, something went wrong..." << endl;
  return 2;
}
