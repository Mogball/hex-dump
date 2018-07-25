package main

import (
	"bufio"
	"errors"
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	if err := run(); err != nil {
		fmt.Println(err.Error())
	}
}

func run() error {
	args := os.Args[1:]
	if len(args) != 2 {
		return errors.New("convert [in] [out]")
	}
	inPath := args[0]
	outPath := args[1]

	inFile, err := os.Open(inPath)
	if err != nil {
		return err
	}
	defer inFile.Close()
	outFile, err := os.Create(outPath)
	if err != nil {
		return err
	}
	defer outFile.Close()

	in := bufio.NewScanner(inFile)
	out := bufio.NewWriter(outFile)
	for in.Scan() {
		line := in.Text()
		parts := strings.Split(line, ",")
		for _, part := range parts {
			if part == "" {
				continue
			}
			val, err := strconv.ParseUint(part, 16, 8)
			if err != nil {
				return err
			}
			if err := out.WriteByte(byte(val)); err != nil {
				return err
			}
		}
	}

	return nil
}
