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

enum Key { DISPLAY_NAME, FROM, TIME, CONTENT };

int main(void) {
  std::unordered_map<std::string, Key> allowed_keys = {
      {"from", FROM}, {"originalarrivaltime", TIME}, {"content", CONTENT}};

  // Хеш-таблица уникальных названий ключей в json, которые нас интересуют

  std::unordered_map<std::string, std::string> authors;
  // Хеш-таблица уникальных авторов ключ это "from", а значение это
  // "displayName"(может быть пустым)

  std::vector<Dialog> dialogs;  //   вектор с диалогами

  // Выделяем буффер чаров у себя под ключи сообщений отсюда проверять будем
  // с строками в allowed_keys. Также этот же буффер используется под значения
  // TODO: надо выяснить макс длину в скайпе
  char buffer[MAX_BUF_SIZE + 1] = {'\0'};

  // Выделяем буффер под отображаемое имя так как в случае сообщения сначала
  // надо будет добраться до "from" проверить его наличие уже в хеш-таблице и
  // если его нет добавить его и отображаемое имя в хеш-таблицу
  char display_name_buffer[MAX_DNAME_SIZE + 1] = {'\0'};

  char symbol = '\0';
  int len_buffer = 0;

  // TODO: открыть файл

  // вместо getchar взять аналог в с++
  while ((symbol = getchar()) != EOF) {
    // встречаем начало ключа
    if (symbol == '"') {
      // перекладываем в буффер символы, пока там есть место и не встретим
      // закрывающие кавычки или конец файла
      while (len_buffer < MAX_KEY_SIZE && (symbol = getchar()) != '"' &&
             symbol != EOF) {
        buffer[len_buffer++] = symbol;
      }
      buffer[len_buffer] = '\0';
      // проверяем ключ на наличие в списке ключей которые нас интересуют
      // if (ключ есть в allowed_keys) {
      //    сохраняем какой это ключ
      //    Key key = allowed_keys[ключ];
      //
      //    char prev_symbol = '\0';
      //    len_buffer =0;
      //    if (getchar() != ':') {
      //        выход?
      //    }
      //    char last_symbol = '"';
      //    if ((symbol = getchar()) == 'n') {
      //        last_symbol = ',';
      //    }
      //
      //    пока не встретим символ last_symbol который не экранируется и не
      //    конец while((symbol = getchar()) != EOF && !(symbol == last_symbol
      //    && prev_symbol == '\')) {
      //        buffer[len_buffer++] = symbol;
      //        prev_symbol = symbol;
      //    }
      //
      //    вот тут надо понять и как-то закинуть получившееся значение
      //    в хеш-таблицу, просто там сейчас я указал хранится указатель
      //    на строку а может все таки просто строку которую тут вот получили
      //    копировать из буффера в хеш-таблицу под соответствующий ключ
      //    ключ - то есть под какое поле класть в структуру message мы
      //    определяем по значению enum из хеш таблицы
      //
      //    вообще лучше отдельную функцию по добавлению
      //    save_token(buffer, dialogs, key);
      // }
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
      // кладем в последнее сообщение последнего диалога в поле from значение из
      // буффера
      break;
      // ...
  }
}
