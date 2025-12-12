# SinricPro Usermod

## Description
Este usermod permite integrar WLED con SinricPro, un servicio de control de dispositivos IoT. Permite controlar la tira de LEDs desde la aplicación SinricPro y configurable desde la interfaz HTML de WLED.

## Features
- Conexión automática a SinricPro usando credenciales configurables
- Control remoto de LEDs desde SinricPro
- Monitoreo de estado de conexión
- Reconexión automática en caso de desconexión
- Configuración persistente de credenciales

## Configuration
El usermod se puede configurar desde la página de configuración de WLED agregando las siguientes opciones:

### Credenciales Iniciales
- **Device ID**: `693c36596ebb39d664cbf65b`
- **App Key**: `cfc93eb7-e6d4-40a7-87ed-d1355aa14da2`
- **App Secret**: `e975af56-388c-4512-88b8-8ccd5fd6c776-9c6cb2a1-7fab-4bcb-926e-a8ce023eb79b`

## Installation
1. Copiar la carpeta `SinricPro_v2` a la carpeta `usermods/`
2. En `platformio.ini`, añadir `SinricPro` a la lista de usermods personalizados (o crear `platformio_override.ini`)
3. Compilar y flashear el firmware
4. Acceder a la configuración de WLED y habilitar/configurar el usermod SinricPro

## JSON Configuration
```json
{
  "SinricPro": {
    "enabled": true,
    "deviceId": "693c36596ebb39d664cbf65b",
    "appKey": "cfc93eb7-e6d4-40a7-87ed-d1355aa14da2",
    "appSecret": "e975af56-388c-4512-88b8-8ccd5fd6c776-9c6cb2a1-7fab-4bcb-926e-a8ce023eb79b"
  }
}
```

## Status
- **Conexión**: Se muestra en la página de información JSON
- **Intentos de reconexión**: Hasta 3 intentos cada 10 segundos
- **Heartbeat**: Se envía automáticamente cada 30 segundos

## Notes
- Requiere conexión WiFi activa
- La configuración se persiste en la EEPROM de WLED
- Compatible con WLED v0.14+
