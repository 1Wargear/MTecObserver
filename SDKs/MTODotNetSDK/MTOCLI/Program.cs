using BetterConsoleTables;
using CommandLine;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.Logging;
using MTOCLI;
using MTOCLI.ConnectVerbs;
using MTODotNetSDK;
using MTODotNetSDK.Channels;
using MTODotNetSDK.DTOs;
using MTODotNetSDK.Interfaces;
using System.Text;
using System.Text.RegularExpressions;

Regex COMMAND_REGEX = new Regex(@"[\w,-:.;-_]+|""[\w\s,-:.;-_]*""");

IConfiguration Configuration = new ConfigurationBuilder()
  .AddJsonFile("appsettings.json", optional: true, reloadOnChange: true)
  .Build();

using var loggerFactory = LoggerFactory.Create(builder =>
{
    builder
        .AddConfiguration(Configuration)
        .AddProvider(new CLILoggerProvider());
});

Parser.Default.ParseArguments<ConnectOptions, MTOCLI.FileOptions>(args)
    .WithParsed<ConnectOptions>(async o => 
    {
        await o.Execute(COMMAND_REGEX, Configuration, loggerFactory);
    })
    .WithParsed<MTOCLI.FileOptions>(o =>
    {

    });

class CLILoggerProvider : ILoggerProvider
{
    class CLILogger : ILogger
    {
        public IDisposable? BeginScope<TState>(TState state) where TState : notnull
        {
            return null;
        }

        public bool IsEnabled(LogLevel logLevel)
        {
            return true;
        }

        public void Log<TState>(LogLevel logLevel, EventId eventId, TState state, Exception? exception, Func<TState, Exception?, string> formatter)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(formatter(state, exception));
            Console.ResetColor();
        }
    }

    public ILogger CreateLogger(string categoryName)
    {
        return new CLILogger();
    }

    public void Dispose()
    {

    }
}
