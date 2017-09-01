// E:\DVD\Datasheets\Specs\USB\USB HID\MSDEV\Projects\test\Multimedia HID -5.h


char ReportDescriptor[43] = {
    0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,                    // USAGE (Consumer Control)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x01,                    //   REPORT_ID (1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x19, 0xb5,                    //   USAGE_MINIMUM (Scan Next Track)
    0x29, 0xb7,                    //   USAGE_MAXIMUM (Stop)
    0x09, 0xcd,                    //   USAGE (Unassigned)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x09, 0xe2,                    //   USAGE (Mute)
    0x81, 0x06,                    //   INPUT (Data,Var,Rel)
    0x95, 0x02,                    //   REPORT_COUNT (2)
    0x09, 0xe9,                    //   USAGE (Volume Up)
    0x09, 0xea,                    //   USAGE (Volume Down)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0xc0                           // END_COLLECTION
};

