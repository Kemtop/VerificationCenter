#ifndef __USB_DESC_H
#define __USB_DESC_H

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE                     0x21
#define HID_REPORT_DESCRIPTOR_TYPE				0x22

#define RHID_SIZ_HID_DESC                   0x09
#define RHID_OFF_HID_DESC                   0x12

#define RHID_SIZ_DEVICE_DESC                18
#define RHID_SIZ_CONFIG_DESC                41
#define RHID_SIZ_REPORT_DESC                148
#define RHID_SIZ_STRING_LANGID              4
#define RHID_SIZ_STRING_VENDOR              42
#define RHID_SIZ_STRING_PRODUCT             24
#define RHID_SIZ_STRING_SERIAL              26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

#define DEVICE_VER_H 0x03
#define DEVICE_VER_L 0x07

//HID Maximum packet size in bytes
#define wMaxPacketSize  0x40
#define EP1TxCount wMaxPacketSize
#define EP1RxCount wMaxPacketSize
#define MaxMessageSize 4096
//#define EP1RxCount 2

/* Exported functions ------------------------------------------------------- */
extern const uint8_t RHID_DeviceDescriptor[RHID_SIZ_DEVICE_DESC];
extern const uint8_t RHID_ConfigDescriptor[RHID_SIZ_CONFIG_DESC];
extern const uint8_t RHID_ReportDescriptor[RHID_SIZ_REPORT_DESC];
extern const uint8_t RHID_StringLangID[RHID_SIZ_STRING_LANGID];
extern const uint8_t RHID_StringVendor[RHID_SIZ_STRING_VENDOR];
extern const uint8_t RHID_StringProduct[RHID_SIZ_STRING_PRODUCT];
extern uint8_t RHID_StringSerial[RHID_SIZ_STRING_SERIAL];

#endif /* __USB_DESC_H */

