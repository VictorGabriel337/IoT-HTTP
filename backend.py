# app.py
from flask import Flask, request, jsonify
import requests

app = Flask(__name__)

ESP_IP = "192.168.15.6"  # IP do seu ESP na rede local

@app.route("/led/<action>", methods=["POST"])
def led_control(action):
    if action not in ["on", "off"]:
        return jsonify({"error": "Ação inválida"}), 400
    try:
        esp_response = requests.get(f"http://{ESP_IP}/led/{action}", timeout=5)
        return jsonify({"status": "ok", "esp_response": esp_response.text})
    except requests.exceptions.RequestException as e:
        return jsonify({"status": "error", "message": str(e)}), 500

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)  # roda na porta 5000
