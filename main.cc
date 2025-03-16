#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// допустим у нас отображаемое имя автора не больше 100 символов
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

enum TypeKey { DIALOG_DNAME, MSG_DNAME, FROM, TIME, CONTENT };

int main(void) {
  // Хеш-таблица уникальных названий ключей в json, которые нас интересуют
  std::unordered_map<std::string, TypeKey> allowed_keys = {
      {"displayName", DIALOG_DNAME},
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
          save_token(buffer, dialogs, typekey);
        }

        // пока не встретим '"', который не экранируется или ',' или конец
           while((symbol = getchar()) != EOF && !(symbol == border_symbol && prev_symbol == '\'))) {
               buffer[len_buffer++] = symbol;
               prev_symbol = symbol;
      }
      if (typekey != DIALOG_DNAME) {
        save_token(buffer, dialogs, key);
      }
    }
  }
  len_buffer = 0;
}

//   for (по количеству диалогов в dialogs) {
//     // TODO: печать каждого диалога в отдельном файле
//     print_dialog();
//   }

return 0;
}

int save_token(char *buffer, std::vector<Dialog> &dialogs, Key key) {
  static bool is_dialog_name = true;
  switch (key) {
    case DISPLAY_NAME:
      if (is_dialog_name) {
        // добавляем диалог в вектор диалогов и ставим ему имя из буффера
        is_dialog_name = false;
      } else {
        // добавляем сообщение в вектор сообщений у диалога последнего и
        // ставим ему имя из буффера
        is_dialog_name = true;  // TODO неправильно!!!
      }
      break;

    case FROM:
      // кладем в последнее сообщение последнего диалога в поле from значение
      // из буффера
      break;
      // ...
  }
}
