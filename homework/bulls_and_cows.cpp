#include <random>
#include <std_lib_facilities.h>

string attempt = "0000";
constexpr int LENGTH_OF_NUMBER = 4;
vector<int> attemptInt (LENGTH_OF_NUMBER);
vector<int> answer (LENGTH_OF_NUMBER);
int bulls = 0;
int cows = 0;


auto generate_answer() {
  // случайный сид
  std::random_device rd;

  // инициализация генератора
  std::mt19937 gen(rd());

  // целочисленное распределение от 0 до 9
  std::uniform_int_distribution<int> distrib(0, 9);

  for (int i = 0; i < LENGTH_OF_NUMBER; ++i) {
    answer[i] = distrib(gen);
  }
}


bool check_attempt() {
  if (attempt.length() != LENGTH_OF_NUMBER) {
      return 0;
  } else {
    for (char i : attempt) {
      if (i < '0' || i > '9') {
        return 0;
      }
    }
  }
  return 1;
}

auto transform_attempt() {
  vector<int> attemptInt(LENGTH_OF_NUMBER);
  for (int i = 0; i < LENGTH_OF_NUMBER; ++i) {
    attemptInt[i] = attempt[i] - '0';
  }
}


auto check() {
  bulls = 0;
  cows = 0;
  vector<int> allNums(10, 0);
  for (int i = 0; i < LENGTH_OF_NUMBER; ++i) {
    ++allNums[answer[i]];
    if (attemptInt[i] == answer[i]) {
      ++bulls;
      --allNums[attemptInt[i]];
    }
  }
  for (int i = 0; i < LENGTH_OF_NUMBER; ++i) {
    if ((allNums[attemptInt[i]] > 0) && (attemptInt[i] != answer[i])) {
      --allNums[attemptInt[i]];
      ++cows;
    }
  }
}


int main() {
  bool guessed = false;

  cout << "=====================================================\n"
      << "             The game \"Bulls and Cows\"\n"
      << "=====================================================\n"
      << "     The computer guessed a 4 unique digit number\n"
      << "                      Guess it!\n"
      << "=====================================================\n"
      << "         Bull - digit in the correct position\n"
      << " Cow - the digit is present, but in the wrong place.\n"
      << "=====================================================\n\n";

  try {
    generate_answer();
    while (!guessed) {
      cout << "Enter your attempt: ";
      cin >> attempt;
      if (check_attempt()) {
        transform_attempt();
        check();
        if (bulls == LENGTH_OF_NUMBER) {
          guessed = true;
          cout << "You guessed the number!\n";
        } else {
          cout << "Bulls: " << bulls << ", Cows: " << cows << std::endl;
        }
      } else {
        cout << "Your attempt must be 4 digits long.\n";
      }
    }
  } catch (exception& e) {
    cout << "Exception: " << e.what() << std::endl;
    return 1;
  }
  cout << "Enter anything to exit\n";
  cin >> attempt;
  return 0;
}
