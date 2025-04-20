#include <string>
/*
Разработать класс Logger, который предоставляет функциональность логирования с возможностью гибкой настройки;

Функциональные требования:
Поддержка добавления обработчиков вывода логов (например, файловые потоки);
Возможность настройки уровня логирования (CRITICAL -> ERROR -> WARNING -> INFO -> DEBUG);
Отправка сообщений в лог с заданным уровнем логирования;
Реализация паттерна "Строитель" (Builder) для создания экземпляра логгера;
Возможность закрытия логгера и корректного завершения работы.

*/
class ILogger {
    public:
        virtual void crit(const std::string& str) = 0;
        virtual void error(const std::string& str) = 0;
        virtual void warn(const std::string& str) = 0;
        virtual void info(const std::string& str) = 0;
        virtual void debug(const std::string& str) = 0;
};


class FileLogger final : ILogger  {
    FileLogger(const std::string& path);
    ~FileLogger();
    void crit(const std::string& str) override;
};

class StreamLogger final : ILogger  {
    StreamLogger(const std::string& path);
    ~StreamLogger();
    void crit(const std::string& str) override;
};