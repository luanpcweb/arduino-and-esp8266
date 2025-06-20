#!/bin/bash

for i in {1..5000}; do
  # Forçar separador decimal com ponto
  temperature=$(LC_NUMERIC=C awk -v min=0 -v max=40 'BEGIN{srand(); print min+rand()*(max-min)}')
  humidity=$(LC_NUMERIC=C awk -v min=40 -v max=90 'BEGIN{srand(); print min+rand()*(max-min)}')
  methane=$(shuf -i 0-1000 -n 1)

  # Montar JSON com jq (mais seguro)
  json=$(jq -n \
    --arg temp "$temperature" \
    --arg hum "$humidity" \
    --arg met "$methane" \
    '{
      temperature: ($temp | tonumber),
      humidity: ($hum | tonumber),
      methane: ($met | tonumber)
    }')

  curl -s -X POST http://localhost:8080/sensor-data?token=abc123 \
    -H "Content-Type: application/json" \
    -d "$json"

  echo " → Enviado [$i]"
  sleep 0.05
done