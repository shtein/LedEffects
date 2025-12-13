try:
    from platformio.public import DeviceMonitorFilterBase
except Exception:  # older cores
    from platformio.device.monitor.filters.base import DeviceMonitorFilterBase

import binascii


class Filter(DeviceMonitorFilterBase):
    NAME = "rx_hex"

    def __init__(self, *args, **kwargs):
        super(Filter, self).__init__(*args, **kwargs)
        # buffer for partial multibyte chars if needed
        self._buf = b""

    def tx(self, text: str) -> str:
        # Just pass keyboard input through unchanged
        return text

    def rx(self, text: str) -> str:
        # text is a decoded string; we want raw bytes
        # encode back using monitor_encoding (latin1 is safe 1:1 mapping)
        data = text.encode("latin1")

        if not data:
            return ""

        # hexlify → b"AA55A55A" ; group into pairs and add spaces
        hexstr = binascii.hexlify(data).decode("ascii").upper()
        spaced = " ".join(hexstr[i : i + 2] for i in range(0, len(hexstr), 2))

        # Add newline to keep things readable
        return hexstr
