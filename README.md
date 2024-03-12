# Mav Shell - Unix Shell Implementation

## Introduction

Welcome to Mav Shell, a Unix shell which provides a command-line interface to interact with the underlying operating system. It supports various features, including command history, process management, and built-in commands.

## Features

- **Command Execution:** Execute commands by typing them into the shell prompt.
- **Command History:** Access and repeat previously executed commands using history functionality.
- **Change Directory:** Use the `cd` command to navigate through directories easily.
- **Show PIDs:** View the list of child process IDs using the `showpids` command.
- **Exit:** Quit the shell using the `quit` or `exit` command.

## Command Options

- **quit / exit:** Terminate the shell.
- **cd:** Change the current working directory. Use `cd ..` to go up one level.
- **history:** Display a list of previously executed commands.
- **showpids:** Show the child process IDs.

## Command History

Mav Shell keeps track of the last 15 executed commands. You can access the command history using the `history` command. To repeat a command from history, use `!` followed by the history number. For example, `!3` will repeat the third command from history.

## Built-in Commands

- **cd [directory]:** Change the current working directory. If no directory is provided, it goes to the home directory. Use `cd ..` to go up one level.
- **history:** Display a list of the last 15 executed commands.
- **showpids:** Show the child process IDs.

## Process Management

Mav Shell provides a simple process management feature. You can view the child process IDs using the `showpids` command.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details. Feel free to use, modify, and distribute the code for your own purposes.
