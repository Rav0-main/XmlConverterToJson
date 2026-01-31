from os import (system,
                remove)
from os.path import (exists,
                     getsize)
from time import perf_counter_ns
from typing import List
from dataclasses import dataclass

@dataclass(frozen=True)
class FieldWidth:
    filename: int
    fileSize: int
    timeInS: int
    repeats: int

@dataclass(frozen=True)
class Row:
    filename: str
    fileSize: str
    timeInS: str
    repeats: str

def create(file: str):
    with open(file, "w"):
        pass

def convertFromXmlToJson(xmlFile: str):
    system(f"{EXE_FILE} {xmlFile} >> {OUTPUT_FILE} 2>&1")

def getFileSize(filename: str, rounding: int) -> str:
    try:
        sizeBytes = getsize(filename)
        
        if(sizeBytes < KB):
            return f"{sizeBytes:.{rounding}f} B"
        
        elif(sizeBytes < MB):
            return f"{sizeBytes/KB:.{rounding}f} KB"
        
        elif(sizeBytes < GB):
            return f"{sizeBytes/MB:.{rounding}f} MB"
        
        else:
            return f"{sizeBytes/GB:.{rounding}f} GB"
    
    except OSError as e:
        return "UNDEFINED"

def outputTable(table: list[Row], width: FieldWidth):
    print("-" * (width.filename + width.fileSize + width.timeInS + width.repeats + 3 + 3 + 3 + 4))
    row = table[0]
    print(f"| {row.filename:^{width.filename}} "+
          f"| {row.fileSize:^{width.fileSize}} "+
          f"| {row.timeInS:^{width.timeInS}} "+
          f"| {row.repeats:^{width.repeats}} |")
    
    print("-" * (width.filename + width.fileSize + width.timeInS + width.repeats + 3 + 3 + 3 + 4))

    for i in range(1, len(table)):
        row = table[i]
        print(f"| {row.filename:<{width.filename}} "+
              f"| {row.fileSize:<{width.fileSize}} "+
              f"| {row.timeInS:<{width.timeInS}} "+
              f"| {row.repeats:<{width.repeats}} |")
        
        print("-" * (width.filename + width.fileSize + width.timeInS + width.repeats + 3 + 3 + 3 + 4))

KB = 1024
MB = 1024*KB
GB = 1024*MB

XML_FILES: List[str] = [
    ".\\small_library.xml", ".\\big_library.xml", ".\\large_library.xml"
]

EXE_FILE: str = ".\\fxmltjson.exe"
if not exists(EXE_FILE):
    raise FileNotFoundError(f"exe-file: {EXE_FILE} must be exists!")

OUTPUT_FILE: str = ".\\output.tmp"

for xmlFile in XML_FILES:
    if not exists(xmlFile):
        raise FileNotFoundError(f"XML-file: {xmlFile} not found! All XML-files must be exists.")

REPEATS: list[int] = [
    10, 5, 1
]

if(len(XML_FILES) != len(REPEATS)):
    raise ValueError(f"len(XML_FILES) must be equals len(REPEATS): {len(XML_FILES)} != {len(REPEATS)}")

MAX_FILENAME_WIDTH = len(max(XML_FILES, key=lambda s: len(s)))

FILENAME_THEMA: str = "FILENAME"
FILE_SIZE_THEMA: str = "FILE SIZE"
TIME_IN_S_THEMA: str = "TIME, s"
REPEATS_THEMA: str = "REPEATS"

MAX_FILENAME_WIDTH = max(len(max(XML_FILES, key=lambda s: len(s))), len(FILENAME_THEMA))

WIDTHS = FieldWidth(
    MAX_FILENAME_WIDTH, max(len(FILE_SIZE_THEMA), 12), max(len(TIME_IN_S_THEMA), 10), 
    max(len(REPEATS_THEMA), 6)
)

FILE_SIZE_ROUNDING: int = 3

if __name__ == "__main__":
    create(OUTPUT_FILE)

    table: list[Row] = [
        Row(FILENAME_THEMA, FILE_SIZE_THEMA, TIME_IN_S_THEMA, REPEATS_THEMA)
    ]

    FROM_NS_TO_MS: float = 10**(-6)
    FROM_NS_TO_S: float = 10**(-9)

    for i in range(len(XML_FILES)):
        xmlFile: str = XML_FILES[i]
        repeats: int = REPEATS[i]
        fileSize: str = getFileSize(xmlFile, FILE_SIZE_ROUNDING)

        summaryTimeInNs: float = 0
        print(f"{i+1}) Testing {xmlFile} by {repeats} repeats...")
        for j in range(repeats):
            start = perf_counter_ns()
            convertFromXmlToJson(xmlFile)
            end = perf_counter_ns()

            summaryTimeInNs += end-start

        table.append(Row(
            xmlFile, fileSize,
            f"{summaryTimeInNs/repeats * FROM_NS_TO_S:.{WIDTHS.timeInS}g}",
            f"{repeats}"
        ))

    remove(OUTPUT_FILE)

    outputTable(table, WIDTHS)