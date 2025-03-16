#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// TODO:  надо выяснить макс длину сообщений в скайпе
#define MAX_BUF_SIZE 10000

typedef struct Message {
  const std::string *display_name;
  const std::string *from;
  std::string time;
  std::string content;
};

typedef struct Dialog {
  std::string name;
  std::vector<Message> messages;
};

enum TypeKey { DNAME_CONV, DNAME_MSG, FROM, TIME, CONTENT };

using AuthorsMap = std::unordered_map<std::string, std::string>;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Usage: skareader <messages.json>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cout << "Can't open file " << argv[1] << "\n";
    return 1;
  }

  std::unordered_map<std::string, TypeKey> allowed_keys = {
      {"displayName", DNAME_CONV},
      {"from", FROM},
      {"originalarrivaltime", TIME},
      {"content", CONTENT}};

  AuthorsMap authors;
  std::vector<Dialog> dialogs;
  char buffer[MAX_BUF_SIZE + 1] = {'\0'};
  int len_buffer = 0;

  char symbol = '\0';
  bool is_last_dname = false;

  // TODO: need try catch
  while ((symbol = file.get()) != EOF) {
    if (symbol == '"') {
      // fill buffer by key
      while (len_buffer < MAX_BUF_SIZE && (symbol = file.get()) != '"' &&
             symbol != EOF) {
        buffer[len_buffer++] = symbol;
      }
      buffer[len_buffer] = '\0';

      // check if key in buffer is interesting
      auto iter = allowed_keys.find(std::string(buffer));
      if (iter != allowed_keys.end()) {
        TypeKey typekey = iter->second;

        if (file.get() != ':') {
          std::cout << "Wrong char after key:" << buffer << " (expected ':')\n";
          throw std::logic_error("Wrong char after key");
        }

        if (typekey == TIME) {
          save_token(buffer, dialogs, DNAME_MSG, authors);
        } else if (is_last_dname) {
          save_token(buffer, dialogs, DNAME_CONV, authors);
          is_last_dname = false;
        }

        // overwrite the same buffer, prepare for value
        len_buffer = 0;
        char border_symbol = '"';

        char prev_symbol = '\0';
        // null case in value
        if ((symbol = getchar()) == 'n') {
          border_symbol = ',';
          buffer[len_buffer++] = symbol;
        }

        while ((symbol = file.get()) != EOF &&
               !(symbol == border_symbol &&
                 prev_symbol == '\\')) {  // TODO: нужно еще null обрабатывать
          buffer[len_buffer++] = symbol;
          prev_symbol = symbol;
        }

        if (typekey != DNAME_CONV) {
          save_token(buffer, dialogs, typekey, authors);
        } else {
          is_last_dname = true;
        }
      }
    }
    len_buffer = 0;
  }

  for (auto &i : dialogs) {
    print_dialog(i);
  }

  file.close();
  return 0;
}

void print_dialog(const Dialog &dialog) {
  std::cout << "========================" << std::endl;
  std::cout << std::endl;
  std::cout << dialog.name << std::endl;
  std::cout << std::endl;
  for (auto &i : dialog.messages) {
    std::cout << i.display_name << std::endl;
    std::cout << i.from << std::endl;
    std::cout << i.content << std::endl;
    std::cout << i.time << std::endl;
    std::cout << std::endl;
  }
  std::cout << "========================" << std::endl;
}

int save_token(char *buffer, std::vector<Dialog> &dialogs, TypeKey key,
               AuthorsMap &authors) {
  static std::string dname_msg;

  switch (key) {
    case DNAME_CONV: {
      Dialog temp = {};
      temp.name = std::string(buffer);
      dialogs.push_back(temp);
      break;
    }

    case DNAME_MSG: {
      dname_msg = std::string(buffer);
      break;
    }

    case FROM:
      if (dialogs.empty() || dialogs.back().messages.empty()) {
        std::cout << "Can't save message: no dialog\n";
        throw std::range_error("Can't add new message");
      } else if (dname_msg.empty()) {
        std::cout << "Can't save message: no dialog name for author\n";
        throw std::logic_error("Can't save message: no dialog name for author");
      }

      auto [iter, inserted] = authors.insert({std::string(buffer), dname_msg});
      if (inserted) {
        dname_msg.clear();
      }
      dialogs.back().messages.back().from = &iter->first;
      dialogs.back().messages.back().display_name = &iter->second;
      break;

    case TIME:
      if (dialogs.empty() || dialogs.back().messages.empty()) {
        throw std::range_error("Can't add new message");
      }
      dialogs.back().messages.back().time = std::string(buffer);
      break;

    case CONTENT:
      if (dialogs.empty() || dialogs.back().messages.empty()) {
        throw std::range_error("Can't add new message");
      }
      dialogs.back().messages.back().content = std::string(buffer);
      break;
  }
}