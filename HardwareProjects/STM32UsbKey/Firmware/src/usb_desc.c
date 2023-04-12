#include "usb_lib.h"
#include "usb_desc.h"


/* USB Standard Device Descriptor */
const uint8_t RHID_DeviceDescriptor[RHID_SIZ_DEVICE_DESC] =
  {
		    RHID_SIZ_DEVICE_DESC,         // ����� ����� ����������� ���������� � ������
		    USB_DEVICE_DESCRIPTOR_TYPE, // bDescriptorType - ����������, ��� ��� �� ����������. � ������ ������ - Device descriptor
		    0x00, 0x02,                 // bcdUSB - ����� ������ ��������� USB ������������ ����������. 2.0

			// �����, �������� ���������� � ��������, �� ��������� USB. � ��� ����, �������� ������ ��������� ��� �� ����
		    0x00,                       //bDeviceClass
		    0x00,                       //bDeviceSubClass
		    0x00,                       //bDeviceProtocol

		    0x40,                       //bMaxPacketSize - ������������ ������ ������� ��� Endpoint 0 (��� ����������������)

			// �� ����� ����������� VID � PID,  �� ������� � ������������, ��� �� ��� �� ����������.
			// � �������� ����������� ���� �������� VID, ����� ���������� ����� ���� ��������� � ����������� ������ ��������
		    0x83, 0x04,                 //idVendor (0x0483)
		    0x11, 0x57,                 //idProduct (0x5711)

		    DEVICE_VER_L, DEVICE_VER_H,                 // bcdDevice rel. DEVICE_VER_H.DEVICE_VER_L  ����� ������ ����������

			// ������ ���� ������� �����, ����������� �������������, ���������� � �������� �����.
			// ������������ � ��������� ���������� � ���������� ���������
			// � �� ��������� ������ ������������ ���������� � ���������� VID/PID ����������� ��������.
		    1,                          //Index of string descriptor describing manufacturer
		    2,                          //Index of string descriptor describing product
		    3,                          //Index of string descriptor describing the device serial number
		    0x01                        // bNumConfigurations - ���������� ��������� ������������. � ��� ����.
  }
  ; /* CustomHID_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t RHID_ConfigDescriptor[RHID_SIZ_CONFIG_DESC] =
  {
		    0x09, 			// bLength: ����� ����������� ������������
		    USB_CONFIGURATION_DESCRIPTOR_TYPE, // bDescriptorType: ��� ����������� - ������������
		    RHID_SIZ_CONFIG_DESC, 0x00, // wTotalLength: ����� ������ ����� ������ ��� ������ ������������� � ������

		    0x01,         // bNumInterfaces: � ������������ ����� ���� ���������
		    0x01,         // bConfigurationValue: ������ ������ ������������
		    0x00,         // iConfiguration: ������ ������, ������� ��������� ��� ������������
		    0xE0,         // bmAttributes: ������� ����, ��� ���������� ����� �������� �� ���� USB
		    0x32,         // MaxPower 100 mA: � ��� ������ 100 ��

				/************** ���������� ���������� ****************/
				0x09,         // bLength: ������ ����������� ����������
				USB_INTERFACE_DESCRIPTOR_TYPE, // bDescriptorType: ��� ����������� - ���������
				0x00,         // bInterfaceNumber: ���������� ����� ���������� - 0
				0x00,         // bAlternateSetting: ������� ��������������� ����������, � ��� �� ������������
				0x02,         // bNumEndpoints - ���������� ����������.

				0x03,         // bInterfaceClass: ����� ��������� - HID
				// ���� �� �� ������ ��� ����������� ����������, �������� ���������� ��� ����, �� ���� ���� �� ������� ��������� ����� � ��������
				// � ��� � ��� ����� HID-����������
				0x00,         // bInterfaceSubClass : �������� ����������.
				0x00,         // nInterfaceProtocol : �������� ����������

				0,            // iInterface: ������ ������, ����������� ���������

					// ������ ��������� ���������� ��� ��������� ����, ��� ������ ��������� - ��� HID ����������
					/******************** HID ���������� ********************/
					0x09,         // bLength: ����� HID-�����������
					HID_DESCRIPTOR_TYPE, // bDescriptorType: ��� ����������� - HID
					0x01, 0x01,   // bcdHID: ����� ������ HID 1.1
					0x00,         // bCountryCode: ��� ������ (���� �����)
					0x01,         // bNumDescriptors: ������� ������ ����� report ������������
						HID_REPORT_DESCRIPTOR_TYPE,         // bDescriptorType: ��� ����������� - report
						RHID_SIZ_REPORT_DESC,	0x00, // wItemLength: ����� report-�����������


					/******************** ���������� �������� ����� (endpoints) ********************/
					0x07,          // bLength: ����� �����������
					USB_ENDPOINT_DESCRIPTOR_TYPE, // ��� ����������� - endpoints

					0x81,          // bEndpointAddress: ����� �������� ����� � ����������� 1(IN)
					0x03,          // bmAttributes: ��� �������� ����� - Interrupt endpoint
					wMaxPacketSize, 0x00,    // wMaxPacketSize:  Bytes max
					0x20,          // bInterval: Polling Interval (32 ms)

          0x07,	/* bLength: Endpoint Descriptor size */
          USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
            /*	Endpoint descriptor type */
          0x01,	/* bEndpointAddress: */
            /*	Endpoint Address (OUT) */
          0x03,	/* bmAttributes: Interrupt endpoint */
          wMaxPacketSize,	/* wMaxPacketSize:  Bytes max  */
          0x00,
          0x20,	/* bInterval: Polling Interval (32 ms) */
}
  ; /* RHID_ConfigDescriptor */
const uint8_t RHID_ReportDescriptor[RHID_SIZ_REPORT_DESC] =
  {
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
		
    0x85, 0xb0,                    //   REPORT_ID (a9)
    0x09, 0xb0,                    //   USAGE (Vendor Usage a9)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x06,                    //   REPORT_COUNT (37)
    0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
    0x85, 0xb0,                    //   REPORT_ID (a9)
    0x09, 0xb0,                    //   USAGE (Vendor Usage a9)
    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)

    0x85, 0xb0,                    //   REPORT_ID (a9)
    0x09, 0xb0,                    //   USAGE (Vendor Usage a9)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
		
		0x85, 0xb1,                    //   REPORT_ID (a9)
    0x09, 0xb1,                    //   USAGE (Vendor Usage a9)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, wMaxPacketSize-1,                    //   REPORT_COUNT ()
    0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
    0x85, 0xb1,                    //   REPORT_ID (a9)
    0x09, 0xb1,                    //   USAGE (Vendor Usage a9)
    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)

    0x85, 0xb1,                    //   REPORT_ID (a9)
    0x09, 0xb1,                    //   USAGE (Vendor Usage a9)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
		
		0x85, 0xb2,                    //   REPORT_ID (a9)
    0x09, 0xb2,                    //   USAGE (Vendor Usage a9)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (37)
    0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
    0x85, 0xb2,                    //   REPORT_ID (a9)
    0x09, 0xb2,                    //   USAGE (Vendor Usage a9)
    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)

    0x85, 0xb2,                    //   REPORT_ID (a9)
    0x09, 0xb2,                    //   USAGE (Vendor Usage a9)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x06,                    //   REPORT_COUNT (4)
    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
		
		0x85, 0xb3,                    //   REPORT_ID (a9)
    0x09, 0xb3,                    //   USAGE (Vendor Usage a9)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x05,                    //   REPORT_COUNT (37)
    0xb1, 0x82,                    //   FEATURE (Data,Var,Abs,Vol)
    0x85, 0xb3,                    //   REPORT_ID (a9)
    0x09, 0xb3,                    //   USAGE (Vendor Usage a9)
    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)

    0x85, 0xb3,                    //   REPORT_ID (a9)
    0x09, 0xb3,                    //   USAGE (Vendor Usage a9)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, wMaxPacketSize-1,                    //   REPORT_COUNT (4)
    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
		
    0x85, 0xff,                    //   REPORT_ID (FF)
    0x09, 0xff,                    //   USAGE (Vendor Usage FF)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)

    0x85, 0xf0,                    //   REPORT_ID (F0)
    0x09, 0xf0,                    //   USAGE (Vendor Usage F0)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)

    0xc0                           // END_COLLECTION
}
  ; /* RHID_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t RHID_StringLangID[RHID_SIZ_STRING_LANGID] =
  {
    RHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t RHID_StringVendor[RHID_SIZ_STRING_VENDOR] =
  {
    RHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'G', 0, 'o', 0, 's', 0, 'k', 0, 'a', 0, 'z', 0,
		'n', 0, 'a', 0, 'c', 0, 'h', 0, 'e', 0, 'y', 0,
		's', 0, 't', 0, 'v', 0, 'o', 0, ' ', 0, 'L', 0,
		'N', 0, 'R', 0
  };

const uint8_t RHID_StringProduct[RHID_SIZ_STRING_PRODUCT] =
  {
    RHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'G', 0, 'r', 0, 'a', 0, 'n', 0, 'i', 0, 't', 0,
    'K', 0, ' ', 0, 'K', 0, 'e', 0, 'y', 0
  };
uint8_t RHID_StringSerial[RHID_SIZ_STRING_SERIAL] =
  {
    RHID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'R', 0, 'H', 0, '-', 0, '0', 0, '0', 0, '0', 0, '1', 0
  };

