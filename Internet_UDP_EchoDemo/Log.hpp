#ifndef __LOG_MODULE__
#define __LOG_MODULE__
#include "Mutex.hpp"
namespace LogModule
{
    const std::string CAGE = "\n";
    class LogFlashStrategy
    {
    public:
        ~LogFlashStrategy() = default ;
        virtual void SyncLog(const std::string& message) = 0; 
    };
    class ConsoleLogStrategy : public LogFlashStrategy
    {
    public:
        void SyncLog(const std::string& logmessage) override
        {
            MutexModule::MutexGuard _MutexGuard (_mutex);
            std::cout<<logmessage<<CAGE ;
        }
    private:
        MutexModule:: Mutex _mutex ;
    };
    const std::string default_filename = "log.txt" ;
    const std::string default_filepath = "./log" ;//./log/log.txt
    class FileLogStrategy : public LogFlashStrategy
    {
    public:
        FileLogStrategy(const std::string file = default_filename , 
            const std::string path = default_filepath )
            :_filename(file),
            _filepath(path)
        {
            MutexModule::MutexGuard _MutexGuard(_mutex);
            if(std::filesystem :: exists(_filepath))
            {
                return ;
            }
            try
            {
                std::filesystem:: create_directories(_filepath);
            }
            catch(const std::filesystem::filesystem_error& _exception)
            {
                std::cout<<_exception.what()<<CAGE ;
            }
        }
        void SyncLog(const std::string& logmessage)
        {
            MutexModule::MutexGuard _MutexGuard(_mutex);
            std::string pathfile = _filepath + 
            (_filepath.back() == '/' ? "" : "/") + _filename ;
            std :: ofstream out (pathfile , std::ios::app);
            if(!out.is_open())
            {
                return ;
            }
            out<<logmessage<<CAGE;
            out.close();
        }
    private:
        MutexModule::Mutex _mutex ;
        std::string _filename ;
        std::string _filepath ;
    };
    enum class LEVEL
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };
    std::string GetCurrentTime()
    {
        time_t cur_time = time(nullptr);
        struct tm result ;
        localtime_r(&cur_time ,&result );
        char timebuffer[128];
        snprintf(timebuffer , sizeof(timebuffer) , "%4d-%02d-%02d %02d:%02d:%02d",
            result.tm_year + 1900,
            result.tm_mon + 1,
            result.tm_mday,
            result.tm_hour,
            result.tm_min,
            result.tm_sec   
        );
        return timebuffer ;
    }
    std::string LevelToString(LEVEL level)
    {
        switch (level)
        {
        case LEVEL::DEBUG:
            return "DEBUG" ;
        case LEVEL::INFO :
            return "INFO" ;
        case LEVEL::WARNING :
            return "WARNING" ;
        case LEVEL::ERROR :
            return "ERROR" ;
        case LEVEL::FATAL :
            return "FATAL"; 
        default :
            return "UNKNOWORNING";
        }
    }
    class Logger
    {
    public:
        Logger()
        {
            EnableConsolLogStrategy();
        }
        void EnableConsolLogStrategy()
        {
            _current_flash_tsrategy = std::make_unique<ConsoleLogStrategy>();
        }
        void EnableFileLogStrategy()
        {
            _current_flash_tsrategy = std::make_unique<FileLogStrategy>();
        }
        class LogMessage
        {
        public:
            LogMessage(LEVEL& level , std::string& file , size_t line , LogModule::Logger& logger)
                :_current_time(GetCurrentTime()),
                _level(level),
                _pid(getpid()),
                _src_file(file),
                _line(line),
                _logger(logger)
            {
                std::stringstream ret ;
                ret<<"["<<_current_time<<"]"
                <<"["<<LevelToString(_level)<<"]"
                <<"["<<_pid<<"]"
                <<"["<<_src_file<<"]"
                <<"["<<_line<<"] -";
                _hole_message = ret.str() ;
            }
            template<class T>
            LogMessage& operator<< (const T& info)
            {
                std::stringstream ss ;
                ss <<info ;
                _hole_message += ss.str() ;
                return *this ;
            }
            ~LogMessage()
            {
                if(_logger._current_flash_tsrategy)
                    _logger._current_flash_tsrategy->SyncLog(_hole_message);
            }
        private:
            std::string _current_time ;
            LEVEL _level ;
            pid_t _pid ;
            std::string _src_file ;
            size_t _line ;
            std::string _hole_message ;
            LogModule::Logger& _logger ;
        };
        LogMessage operator()(LEVEL level , std::string filename , size_t line  )
        {
            return LogMessage(level , filename ,line,*this); 
        }
    private:
        std::unique_ptr<LogFlashStrategy> _current_flash_tsrategy ;
    };
    Logger logger ;
#define LOG(level) logger(level , __FILE__ , __LINE__)
#define ENABLE_CONSOLE_LOG_STRATEGY() logger.EnableConsolLogStrategy()
#define ENABLE_FILE_LOG_STRATEGY() logger.EnableFileLogStrategy()
}
#endif