try:
    from platformio.public import DeviceMonitorFilterBase
except Exception:  # older cores
    from platformio.device.monitor.filters.base import DeviceMonitorFilterBase

import binascii, re

class Filter(DeviceMonitorFilterBase):
    NAME = "tx_hex"

    def tx(self, text):
        s = re.sub(r"\s+", "", text)  # allow "AA 55 A5 5A"
        if not s or len(s) % 2 or not re.fullmatch(r"[0-9A-Fa-f]+", s):
            return text               # not hex -> pass through
        raw = binascii.unhexlify(s)   # bytes
        return raw.decode("latin1")   # return *str* (PIO will encode with monitor_encoding)

    def rx(self, text):
        return text                   # leave device output as text


