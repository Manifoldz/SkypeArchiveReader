#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
// hu
//  допустим у нас отображаемое имя автора не больше 100 символов
#define MAX_DNAME_SIZE 100
// допустим у нас контент не больше 10000 символов TODO: это надо выяснить макс
// длину сообщения в скайпе
#define MAX_BUF_SIZE 10000

typedef struct Message {
  std::string display_name;
  std::string from;
  std::string time;
  std::string content;
};

typedef struct Dialog {
  std::string name;
  std::vector<Message> messages;
};

enum TypeKey { DNAME_CONV, DNAME_MSG, FROM, TIME, CONTENT };

int main(void) {
  // Хеш-таблица уникальных названий ключей в json, которые нас интересуют
  std::unordered_map<std::string, TypeKey> allowed_keys = {
      {"displayName", DNAME_CONV},
      {"from", FROM},
      {"originalarrivaltime", TIME},
      {"content", CONTENT}};

  // Хеш-таблица уникальных авторов, ключ это "from", а значение "displayName"
  std::unordered_map<std::string, std::string> authors;

  // Все диалоги
  std::vector<Dialog> dialogs;

  // Выделяем буффер под ключи сообщений - отсюда будем сверять их
  // с строками в allowed_keys. Также этот же буффер используется для значений
  // TODO: надо выяснить макс длину в скайпе
  char buffer[MAX_BUF_SIZE + 1] = {'\0'};

  char symbol = '\0';
  int len_buffer = 0;
  bool is_last_dname = false;

  // TODO: открыть файл

  // вместо getchar взять аналог в с++
  while ((symbol = getchar()) != EOF) {
    // встречаем начало ключа
    if (symbol == '"') {
      // перекладываем в буффер символы, пока там есть место и не встретим
      // закрывающие кавычки или конец файла
      while (len_buffer < MAX_BUF_SIZE && (symbol = getchar()) != '"' &&
             symbol != EOF) {
        buffer[len_buffer++] = symbol;
      }
      buffer[len_buffer] = '\0';

      // проверяем ключ на наличие в списке ключей которые нас интересуют
      auto iter = allowed_keys.find(std::string(buffer));
      if (iter != allowed_keys.end()) {
        // сохраняем какой это ключ из enum
        TypeKey typekey = iter->second;

        // выходим если не двоеточие
        if (getchar() != ':') {
          std::cout << "Wrong char after key:" << buffer << "\n";
          return 1;
        }

        char border_symbol = '"';  // до какого символа читаем значение
        if ((symbol = getchar()) == 'n') {
          border_symbol = ',';
        }

        char prev_symbol = '\0';
        len_buffer = 0;  // переиспользуем буффер

        if (typekey == TIME) {
          save_token(buffer, dialogs, DNAME_MSG);
        } else if (is_last_dname) {
          save_token(buffer, dialogs, DNAME_CONV);
          is_last_dname = false;
        }

        // пока не встретим '"', который не экранируется или ',' или конец
        while ((symbol = getchar()) != EOF &&
               !(symbol == border_symbol && prev_symbol == '\\')) {
          buffer[len_buffer++] = symbol;
          prev_symbol = symbol;
        }

        if (typekey != DNAME_CONV) {
          save_token(buffer, dialogs, typekey);
        } else {
          is_last_dname = true;
        }
      }
    }
    len_buffer = 0;
  }

  for (auto &i : dialogs) {
    // TODO: печать каждого диалога в отдельном файле
    print_dialog(i);
  }

  return 0;
}

// void print_dialog(const Dialog &dialog) { cout << dialog.name << "\n"; }

int save_token(char *buffer, std::vector<Dialog> &dialogs, Key key) {
  switch (key) {
    case DNAME_CONV: {
      Dialog temp = {};
      temp.name = std::string(buffer);
      dialogs.push_back(temp);
      break;
    }

    case DNAME_MSG: {
      Message name = {};
      name.display_name = std::string(buffer);
      dialogs.back().messages.push_back(name);
      break;
    }

    case FROM:
      dialogs.back().messages.back().from = std::string(buffer);
      break;

    case TIME:
      dialogs.back().messages.back().time = std::string(buffer);
      break;

    case CONTENT:
      dialogs.back().messages.back().content = std::string(buffer);
      break;
  }
}
