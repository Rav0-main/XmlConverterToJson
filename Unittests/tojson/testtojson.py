from json import (load,
                  JSONDecodeError)
from typing import AnyStr
from dataclasses import dataclass
from enum import IntEnum
from unittest import (TestCase,
                      main)

class IntParsingResult(IntEnum):
    Success = 0
    FileNotExistsError = 1
    JsonLoadingError = 2
    UnknownError = 3

@dataclass(frozen=True)
class ParsedJson:
    result: IntParsingResult
    data: dict

def getFromJson(filename: AnyStr) -> ParsedJson:
    try:
        with open(filename, "r") as file:
            data = load(file)

        return ParsedJson(
            IntParsingResult.Success, data
        )

    except FileExistsError:
        return ParsedJson(
            IntParsingResult.FileNotExistsError, {}
        )
    
    except JSONDecodeError:
        return ParsedJson(
            IntParsingResult.JsonLoadingError, {}
        )
    
    except Exception:
        return ParsedJson(
            IntParsingResult.UnknownError, {}
        )
    
class TestOfConvertingTreeToJson(TestCase):
    filename: str
    
    def assertParsedJson(self, answer: ParsedJson, value: ParsedJson):
        self.assertEqual(answer.result, value.result, 
                         f"Parsing result must be {answer.result},\n \
                            but got {value.result}!")
        
        self.assertDictEqual(answer.data, value.data)

    def testEmptyTree(self):
        self.filename: str = "empty_tree.json"
        empty_tree = getFromJson(self.filename)

        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, {}),
            empty_tree
        )

    def test1Tree(self):
        self.filename: str = "1_tree.json"
        answer: dict = {'main': {'t1': 'tag 1', 
                                 't2': {
                                     't2.1': 'tag 2.1',
                                     't2.2': {
                                         't2.2.1': 'tag 2.2.1',
                                         'empty': ''
                                         }, 
                                      'empty': ''
                                      },
                                'NOT EMPTY': 'MEGA VAlUE'}
                        }
        
        tree = getFromJson(self.filename)

        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            tree
        )

    def test2Trees(self):
        self.filename: str = "2_trees.json"
        answer: dict = {'root1': {
                        't1': 'tag 1',
                        't2': {
                            't2.1': 'tag 2.1',
                            't2.2': 'tag 2.2'
                        },
                        'empty': ''},
                        'root2': {
                            'empty': '',
                            't1': 'tag 1'
                        }
                        }
        
        trees = getFromJson(self.filename)

        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            trees
        )

    def test3Trees(self):
        self.filename: str = "3_trees.json"
        answer: dict = {'root1': '',
                        'HELLO': '',
                        'root3': {'t1': 'tag 1',
                                  't2': 'tag 2'
                                  }
                        }
        
        trees = getFromJson(self.filename)

        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            trees
        )

if __name__ == "__main__":
    main()