package main

import (
	"encoding/hex"
	"example/example"
	"fmt"
	"io"
	"log"
	"os"

	"google.golang.org/protobuf/proto"
)

func main() {
	// 打开文件
	file, err := os.Open("../HEX_MSG.txt")
	if err != nil {
		log.Fatalf("无法打开文件: %v", err)
	}
	defer file.Close()

	// 读取文件内容
	data, err := io.ReadAll(file)
	if err != nil {
		log.Fatalf("无法读取文件: %v", err)
	}

	// 去掉可能存在的空白符或换行符
	hexString := string(data)
	hexString = removeWhitespace(hexString)

	// 将十六进制字符串转换为字节数组
	bytes, err := hex.DecodeString(hexString)
	if err != nil {
		log.Fatalf("无法解析十六进制字符串: %v", err)
	}

	// 创建一个 example 实例
	ex := &example.Example{} // 修改为正确的消息类型

	// 使用 Protobuf 解码数据
	err = proto.Unmarshal(bytes, ex)
	if err != nil {
		log.Fatalf("Protobuf 解码失败: %v", err)
	}

	// 打印解码后的对象
	printExample(ex)
}

// removeWhitespace 移除字符串中的所有空白符
func removeWhitespace(s string) string {
	result := ""
	for _, char := range s {
		if char != ' ' && char != '\n' && char != '\t' && char != '\r' {
			result += string(char)
		}
	}
	return result
}

// printExample 逐行打印 Example 对象的字段值
func printExample(ex *example.Example) {
	fmt.Println("Example Message:")
	fmt.Printf("MAC: %s\n", ex.GetMac())
	fmt.Printf("Timestamp: %d\n", ex.GetTimestamp())
	fmt.Printf("Exec File: %s\n", hex.EncodeToString(ex.GetExecFile()))
	fmt.Printf("Lib Files: %v\n", formatBytesSlice(ex.GetLibFile()))
	fmt.Printf("Algorithm: %s\n", ex.GetAlgorithm())
	fmt.Printf("Supported Algorithms: %v\n", ex.GetSupportedAlgorithm())
	fmt.Printf("Publisher: %v\n", ex.GetPublisher())
	fmt.Printf("Contributers: %v\n", ex.GetContributer())
	fmt.Printf("Devices: %v\n", ex.GetDevice())
	fmt.Printf("Sea: %d\n", ex.GetSea())
}

// formatBytesSlice 格式化 [][]byte 列表为字符串
func formatBytesSlice(b [][]byte) string {
	var result string
	for _, byteSlice := range b {
		result += hex.EncodeToString(byteSlice) + " "
	}
	return result
}
