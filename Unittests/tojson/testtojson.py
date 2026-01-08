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
        empty_root = getFromJson(self.filename)

        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, {}),
            empty_root
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
        
        root = getFromJson(self.filename)

        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            root
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
        
        roots = getFromJson(self.filename)

        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            roots
        )

    def test3Trees(self):
        self.filename: str = "3_trees.json"
        answer: dict = {'root1': '',
                        'HELLO': '',
                        'root3': {'t1': 'tag 1',
                                  't2': 'tag 2'
                                  }
                        }
        
        roots = getFromJson(self.filename)

        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            roots
        )

    def testSimpleOneNamedArray(self):
        self.filename: str = ".\\simple_one_named_array.json"
        answer: dict = {"main": {
                        "people": [
                            "Ivan", "Georgy", "John",
                            "My friend"
                        ]
                        }
                        }
        
        root = getFromJson(self.filename)
        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            root
        )

    def testObjectOneNamedArray(self):
        self.filename: str = ".\\object_one_named_array.json"
        answer: dict = {"main": {
                        "people": [
                            {
                                "name": "Ivan",
                                "surname": "Shweps"
                            },
                            {
                                "name": "Georgy",
                                "surname": "First"
                            },
                            {
                                "name": "Friend",
                                "surname": "Unknown"
                            }
                        ]
                        }
                        }
        
        root = getFromJson(self.filename)
        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            root
        )

    def testTwoDifferentArraysInTag(self):
        self.filename: str = ".\\two_arrays_in_tag.json"
        answer: dict = {"main":{
                        "people":{
                            "human":[
                                {
                                    "name": "Ivan",
                                    "surname": "Shweps"
                                },
                                {
                                    "name": "Georgy",
                                    "surname": "First"
                                },
                                {
                                    "name": "Friend",
                                    "surname": "Unknown"
                                }
                            ],
                            "only_name": [
                                "Ivan",
                                "Georgy",
                                "John",
                                "My friend"
                            ]
                        }
                        }
                        }
        
        root = getFromJson(self.filename)
        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            root
        )

    def testNestedDifferentArrays(self):
        self.filename: str = ".\\nested_arrays.json"
        answer: dict = {
	        "main" : [
		        {
			        "only_name" : [
				        "Ivan",
				        "Georgy",
				        "John",
				        "My friend"
			        ],
			        "human" : [
				        {
					        "name" : "Ivan",
					        "surname" : "Shweps"
				        },
				        {
					        "name" : "Georgy",
					        "surname" : "First"
				        },
				        {
					        "name" : "Friend",
					        "surname" : "Unknown"
				        }
			        ]
		        },
		        [
			        "w1",
			        "w2",
			        "w3"
		        ],
		        "tag value"
	        ]
        }

        root = getFromJson(self.filename)
        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            root
        )

    def testArrayWithNotAllSubarrays(self):
        self.filename: str = ".\\not_all_subarrays.json"
        answer: dict = {"main" : {
		                "array" : {
			                "fourth" : "5",
	                        "third" : {
				    	        "name" : "AMAZING",
					            "surname" : "WOOOOW!"
				            },
			                "second" : "2",
			                "first" : [
				                "1",
				                "4"
                            ]
		                }
	                }}
        
        root = getFromJson(self.filename)
        self.assertParsedJson(
            ParsedJson(IntParsingResult.Success, answer),
            root
        )  

if __name__ == "__main__":
    main()