package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
)

type SensorData struct {
	Temperature float64 `json:"temperature"`
	Humidity    float64 `json:"humidity"`
	Methane     float64 `json:"methane"`
}

var apiToken = os.Getenv("TOKEN")

func writeToInflux(data SensorData) {
	client := influxdb2.NewClient(os.Getenv("INFLUX_URL"), os.Getenv("INFLUX_TOKEN"))
	defer client.Close()

	writeAPI := client.WriteAPIBlocking(os.Getenv("INFLUX_ORG"), os.Getenv("INFLUX_BUCKET"))

	point := influxdb2.NewPointWithMeasurement("iot_data").
		AddField("temperature", data.Temperature).
		AddField("humidity", data.Humidity).
		AddField("methane", data.Methane).
		SetTime(time.Now())

	err := writeAPI.WritePoint(context.Background(), point)
	if err != nil {
		log.Printf("Error ao gravar no influxdb: %v", err)
	}
}

func sensorHandler(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		http.Error(w, "Metodo nao suportado", http.StatusBadRequest)
		return
	}

	token := r.URL.Query().Get("token")
	if token != apiToken {
		http.Error(w, "Token invalido ou ausente", http.StatusUnauthorized)
		return
	}

	var data SensorData
	if err := json.NewDecoder(r.Body).Decode(&data); err != nil {
		http.Error(w, "Error ao decodificar JSON", http.StatusBadRequest)
		return
	}

	writeToInflux(data)
	w.WriteHeader(http.StatusOK)
	fmt.Fprint(w, "Dados recebidos com sucesso")
}

func main() {
	http.HandleFunc("/sensor-data", sensorHandler)
	fmt.Println("Servidor rodando na porta 8080...")
	log.Fatal(http.ListenAndServe(":8080", nil))
}
