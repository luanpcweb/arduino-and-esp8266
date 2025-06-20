## API



InfluxDB: http://localhost:8086
→ Login: admin / admin123

Grafana: http://localhost:3000
→ Login padrão: admin / admin123



curl -X POST http://localhost:8080/sensor-data?token=abc123 \
  -H "Content-Type: application/json" \
  -d '{"temperature": 23.5, "humidity": 55, "methane": 340}'



docker exec -it influxdb bash


  influx query '
from(bucket: "sensor-bucket")
  |> range(start: -10m)
  |> filter(fn: (r) => r._measurement == "iot_data")
  |> limit(n: 5)
' --org sensor --token xyz123


### Grafana 

from(bucket: "sensor-bucket")
  |> range(start: -5m)
  |> filter(fn: (r) => r._measurement == "iot_data")
  |> filter(fn: (r) => r._field == "temperature" or r._field == "humidity" or r._field == "methane")
