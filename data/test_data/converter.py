import csv
from typing import TypedDict, cast
from pathlib import Path


class OutData(TypedDict):
    t_us: str
    ax: str
    ay: str
    az: str
    gx: str
    gy: str
    gz: str
    mx: str
    my: str
    mz: str
    imuTempC: str
    baroTempC: str
    pressPa: str
    altM: str
    hgx: str
    hgy: str
    hgz: str


SCRIPT_DIR = Path(__file__).resolve().parent
ROOT_DIR = SCRIPT_DIR.parent


tf_hr_path = ROOT_DIR / "FAR Test Flight" / "Primary Raven Data" / "Primary_highrate.csv"
tf_lr_path = ROOT_DIR / "FAR Test Flight" / "Primary Raven Data" / "Primary_lowrate.csv"
write_file_path = SCRIPT_DIR / "test_data.csv"


def convert_dicts(hr: dict, lr: dict) -> OutData:
    data: OutData = cast(OutData, {})

    def stous(secs: str) -> str:
        us = float(secs) * 1000000
        return str(round(us))

    def gtomps(gs: str) -> str:
        mps = float(gs) * 9.80665
        return str(round(mps, 6))

    def dtor(degs: str) -> str:
        r = float(degs) * 0.017453
        return str(round(r, 6))

    def satopa(sa: str) -> str:
        pa = float(sa) * 101325
        return str(round(pa, 6))

    def fttom(ft: str) -> str:
        m = float(ft) * 0.3048
        return str(round(m, 2))

    try:
        data["t_us"] = stous(hr["Flight_Time_(s)"])
        data["ax"] = gtomps(hr["Accel_X"])
        data["ay"] = gtomps(hr["Accel_Y"])
        data["az"] = gtomps(hr["Accel_Z"])
        data["gx"] = dtor(hr["Gyro_X"])
        data["gy"] = dtor(hr["Gyro_Y"])
        data["gz"] = dtor(hr["Gyro_Z"])
        data["mx"] = "0.0"
        data["my"] = "0.0"
        data["mz"] = "0.0"
        data["imuTempC"] = "0.0"
        data["baroTempC"] = "0.0"
        data["pressPa"] = satopa(lr["Baro_Press_(atm)"])
        data["altM"] = fttom(lr["Baro_Altitude_ASL_(feet)"])
        data["hgx"] = data["ax"]
        data["hgy"] = data["ay"]
        data["hgz"] = data["az"]

        return data
    except KeyError:
        RuntimeError("Faulty data passed in. Check convert_dicts()")


def parse_highrate_line(line: str):
    pass


def equal_time(hr: dict, lr: dict) -> bool:
    try:
        hr_time = hr["Flight_Time_(s)"]
        lr_time = lr["Flight_Time_(s)"] + "0"
        return hr_time == lr_time
    except KeyError:
        RuntimeError("Faulty data passed in to check_time()")


if (__name__ == "__main__"):
    hr = open(tf_hr_path, newline='')
    lr = open(tf_lr_path, newline='')
    wf = open(write_file_path, "w", newline='')

    hr_reader = csv.DictReader(hr)
    lr_reader = csv.DictReader(lr)
    data_keys = list(OutData.__annotations__.keys())
    writer = csv.DictWriter(wf, fieldnames=data_keys)
    writer.writeheader()

    hr_line = next(hr_reader, None)
    lr_line = next(lr_reader, None)
    while hr_line and lr_line:
        if not equal_time(hr_line, lr_line):
            hr_line = next(hr_reader, None)
            continue

        writer.writerow(convert_dicts(hr_line, lr_line))
        hr_line = next(hr_reader, None)
        lr_line = next(lr_reader, None)

    hr.close()
    lr.close()
    wf.close()
