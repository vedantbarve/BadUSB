package main

import (
	"bufio"
	"fmt"
	"net"
	"net/http"
	"os"
	"sync"
	"time"
)

func GetLocalIP() string {
	conn, err := net.Dial("udp", "8.8.8.8:80")
	if err != nil {
		fmt.Println("Error : ", err)
	}
	defer conn.Close()
	localAddress := conn.LocalAddr().(*net.UDPAddr)
	return localAddress.IP.String()
}

func SocketServer(ipAddr string) {
	server, err := net.Listen("tcp", ipAddr+":9999")
	if err != nil {
		panic(err)
	}

	conn, err := server.Accept()
	if err != nil {
		panic(err)
	}

	for {
		fmt.Println("> ")
		inputReader := bufio.NewReader(os.Stdin)
		input, _ := inputReader.ReadString('\n')
		input = string(len(input)) + input
		conn.Write([]byte(input))

		if input == "exit" {
			break
		}

		buffer := make([]byte, 2048)
		conn.Read(buffer)
		fmt.Println(string(buffer))
	}

	defer conn.Close()
}

func HttpServer(ipAddr string) {
	hostedOn := ipAddr + ":9998"

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		file, err := os.Open("../client/client.exe")
		if err != nil {
			http.Error(w, "File not found", http.StatusNotFound)
			return
		}
		defer file.Close()

		w.Header().Set("Content-Disposition", "attachment; filename=client.exe")
		w.Header().Set("Content-Type", "application/octet-stream")
		http.ServeContent(w, r, "client.exe", time.Now(), file)
	})
	http.ListenAndServe(hostedOn, nil)
}

func main() {
	var wg sync.WaitGroup

	ipAddr := GetLocalIP()

	wg.Add(1)
	go SocketServer(ipAddr)

	wg.Add(2)
	go HttpServer(ipAddr)

	wg.Wait()
}
