#include <random>
#include <std_lib_facilities.h>
#include <string_view>

constexpr int LENGTH = 4;

int count (const vector<char>& digits, char d)
{
  int k{};

  for (int i = 0; i < LENGTH; ++i)
    if (digits[i] == d)
      ++k;

  return k;
}

void validate (const vector<char>& number)
{
  for (int i = 0; i < LENGTH; ++i)
  {
    if (number[i] < '0' || '9' < number[i])
    {
      error("the number contains not a digit");
    }

    if (count(number, number[i]) != 1)
    {
      error("digits of the number are not unique");
    }
  }
}

vector<char> user_guess ()
{
  vector<char> number(LENGTH);
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
      for (int i = 0; i < LENGTH; ++i)
      {
        number[i] = str[i];
      }
    }
  }
  else if (!cin)
  {
    error("invalid input");
  }
  validate(number);
  return number;
}

auto generate_number ()
{
  vector<char> number(LENGTH);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(0, 9);

  int i = 0;
  while (i < LENGTH)
  {
    char n = '0' + distrib(gen);
    if (count(number, n) == 0)
    {
      number[i] = n;
      ++i;
    }
  }
  return number;
}

void start_game ()
{
  vector<char> number = generate_number();
  int bulls{};
  do
  {
    bulls = 0;
    int cows{};
    try
    {
      vector<char> uguess = user_guess();
      for (int i = 0; i < static_cast<int>(uguess.size()); ++i)
      {
        if (uguess[i] == number[i])
          ++bulls;
        else if (count(number, uguess[i]) == 1)
          ++cows;
      }
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
    cout << bulls << " bull(s) and " << cows << " cow(s)" << endl;
  }
  while (bulls != 4);
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
    cout << "game is over" << endl;
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
