# TTGO LoRa32-OLED MQTT 기반 밸브 제어 시스템

## 개요

이 프로젝트는 TTGO LoRa32-OLED 모듈을 사용하여 MQTT 프로토콜을 통해 밸브 시스템을 제어하는 예제입니다. Wi-Fi를 통해 MQTT 브로커(HiveMQ Cloud)에 연결하며, 두 개의 밸브(좌/우)를 제어할 수 있습니다.

## 주요 기능

* Wi-Fi 연결을 통해 HiveMQ Cloud MQTT 브로커에 연결
* MQTT 주제(topic) 구독: `valve/1/right` 및 `valve/1/left`
* MQTT 메시지 수신 시 밸브 상태 제어 (ON/OFF)
* OLED 디스플레이를 통해 시스템 상태 표시 (향후 확장 가능)

## 하드웨어 요구사항

* TTGO LoRa32-OLED (V1)
* Wi-Fi 네트워크
* 두 개의 제어 밸브 (PIN 25: LEFT, PIN 21: RIGHT)

## 소프트웨어 설정

1. Arduino IDE 설치

2. TTGO LoRa32-OLED 보드 설정:

   * 보드: TTGO LoRa32-OLED
   * 포트: 적절한 COM 포트 선택
   * Flash Frequency: 80MHz
   * Upload Speed: 115200
   * Core Debug Level: "None"

3. 라이브러리 설치:

   * WiFi.h
   * WebServer.h
   * Adafruit\_GFX.h
   * Adafruit\_SSD1306.h
   * WiFiClientSecure.h
   * PubSubClient.h

## 코드 설명

* Wi-Fi 연결:

  * `setup_wifi()` 함수에서 지정된 SSID 및 비밀번호로 Wi-Fi 네트워크에 연결
* MQTT 설정:

  * MQTT 브로커: HiveMQ Cloud (TLS 포트 8883)
  * 구독 주제: `valve/1/right`, `valve/1/left`
  * 수신 메시지에 따른 밸브 제어 (ON/OFF)
* 콜백 함수: `mqttCallback()`에서 수신된 MQTT 메시지를 처리하여 지정된 밸브 제어

## 설치 및 실행

1. Arduino IDE에서 코드를 열고 업로드
2. Wi-Fi 네트워크와 MQTT 브로커 설정 확인
3. Serial Monitor에서 연결 상태 확인

## 문제 해결

* Wi-Fi 연결 실패 시 30초 후 자동 재시도
* MQTT 연결 실패 시 5초 후 자동 재시도

## 라이선스

이 프로젝트는 자유롭게 사용 및 수정할 수 있으며, 상업적 사용은 허용됩니다.

## 문의

추가 문의 사항은 이메일로 연락해주세요.
