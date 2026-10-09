#include <algorithm>
#include <random>
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
      int p = 1;
      for (int k = 0; k < LENGTH - 1 - i; ++k)
        p *= 10;
      number[i] = (a / p) % 10;
    }
  }

  void update (string a)
  {
    for (int i{0}; i < LENGTH; ++i)
      number[i] = a[i] - '0';
  }

  bool validate ()
  {
    for (int i{0}; i < LENGTH; ++i)
      for (int j{i + 1}; j < LENGTH; ++j)
        if (number[i] == number[j])
          return false;
    return true;
  }

  string toString ()
  {
    string result = "";
    for (int i{0}; i < LENGTH; ++i)
      result += to_string(number[i]);
    return result;
  }

  bool operator== (const num& other)
  {
    for (int i{0}; i < LENGTH; ++i)
      if (number[i] != other.number[i])
        return false;
    return true;
  }
};

class BullsAndCowsSolver
{
private:
  vector<string> candidates;
  int attempts = 0;

  void generateAllNumbers ()
  {
    for (int a = 0; a <= 9; ++a)
      for (int b = 0; b <= 9; ++b)
      {
        if (b == a)
          continue;
        for (int c = 0; c <= 9; ++c)
        {
          if (c == a || c == b)
            continue;
          for (int d = 0; d <= 9; ++d)
          {
            if (d == a || d == b || d == c)
              continue;
            string s;
            s += char('0' + a);
            s += char('0' + b);
            s += char('0' + c);
            s += char('0' + d);
            candidates.push_back(s);
          }
        }
      }
  }

  std::pair<int, int> calculate (const string& guess, const string& secret)
  {
    int bulls = 0, cows = 0;
    for (int i{0}; i < LENGTH; ++i)
    {
      for (int j{0}; j < LENGTH; ++j)
      {
        if (guess[i] == secret[j])
        {
          if (i == j)
            ++bulls;
          else
            ++cows;
        }
      }
    }
    return {bulls, cows};
  }

  void filter (const string& guess, int bulls, int cows)
  {
    vector<string> filtered;
    for (const auto& cand : candidates)
    {
      auto [b, c] = calculate(guess, cand);
      if (b == bulls && c == cows)
        filtered.push_back(cand);
    }
    candidates = std::move(filtered);
  }

public:
  BullsAndCowsSolver () { generateAllNumbers(); }

  int getAttempts () const { return attempts; }

  size_t remaining () const { return candidates.size(); }

  bool empty () const { return candidates.empty(); }

  string makeGuess ()
  {
    ++attempts;
    return candidates[0];
  }

  void reportResult (const string& guess, int bulls, int cows)
  {
    filter(guess, bulls, cows);
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
    for (int j{0}; j < LENGTH; ++j)
      if (refNum.number[i] == compNum.number[j])
      {
        if (i == j)
          ++bullsCows[0];
        else
          ++bullsCows[1];
      }
}

num user_guess ()
{
  string str = "";
  cout << "guess the number: ";
  if (cin >> str)
  {
    if (str.size() > LENGTH)
      error("the number contains too many characters");
    else if (str.size() < LENGTH)
      error("the number contains too few characters");
    else
    {
      for (int i{0}; i < LENGTH; ++i)
        if (str[i] < '0' || '9' < str[i])
          error("the number contains not a digit");

      num number{str};
      if (number.validate())
        return number;
      else
        error("digits of the number are not unique");
    }
  }
  error("invalid input");
}

void start_game ()
{
  // Компьютер загадывает число
  num botNumber{0};
  do
  {
    botNumber.update(generate_number());
  }
  while (!botNumber.validate());

  BullsAndCowsSolver bot;

  vector<int> bullsCows(2);

  while (true)
  {
    // ход пользователя
    num uguess{0};
    while (true)
    {
      try
      {
        uguess = user_guess();
        break;
      }
      catch (const exception& e)
      {
        if (e.what() == "invalid input")
        {
          error(e.what());
          break;
        }
        cerr << e.what() << endl;
        continue;
      }
    }

    countBullsAndCows(bullsCows, botNumber, uguess);
    cout << bullsCows[0] << " bull(s) and " << bullsCows[1] << " cow(s)"
         << endl;

    if (bullsCows[0] == 4)
    {
      cout << "you win!" << endl;
      return;
    }

    // ход компьютера
    string botGuess = bot.makeGuess();
    cout << "My move: " << botGuess << "? (y/n): ";

    while (true)
    {
      string t;
      cin >> t;

      if (t == "y")
      {
        cout << "bot win, ha-ha" << endl;
        return;
      }
      if (t == "n")
        break;
      cout << "invalid answer. (y/n): ";
    }

    int botBulls{0}, botCows{0};
    cout << "Bulls: ";
    if (!(cin >> botBulls))
      error("invalid input");
    cout << "Cows: ";
    if (!(cin >> botCows))
      error("invalid input");

    if (botBulls == 4)
    {
      cout << "bot win, ha-ha" << endl;
      return;
    }

    bot.reportResult(botGuess, botBulls, botCows);

    if (bot.empty())
    {
      cout << "Contradictory answers - no such number exists.\n";
      return;
    }
  }
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