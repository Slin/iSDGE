// ================================================================================================
//  sgXML.h, heavily modified TBXML.h
//  Fast processing of XML files
//
// ================================================================================================
//  Created by Tom Bradley on 21/10/2009.
//  Version 1.3
//  
//  Copyright (c) 2009 Tom Bradley
//  
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
// ================================================================================================

#ifndef __SGXML_H__
#define __SGXML_H__

// ================================================================================================
//  Defines
// ================================================================================================
#define MAX_ELEMENTS 100
#define MAX_ATTRIBUTES 100

#define TBXML_ATTRIBUTE_NAME_START 0
#define TBXML_ATTRIBUTE_NAME_END 1
#define TBXML_ATTRIBUTE_VALUE_START 2
#define TBXML_ATTRIBUTE_VALUE_END 3
#define TBXML_ATTRIBUTE_CDATA_END 4

// ================================================================================================
//  Structures
// ================================================================================================
typedef struct _TBXMLAttribute {
	char * name;
	char * value;
	struct _TBXMLAttribute * next;
} TBXMLAttribute;

typedef struct _TBXMLElement {
	char * name;
	char * text;
	
	TBXMLAttribute * firstAttribute;
	
	struct _TBXMLElement * parentElement;
	
	struct _TBXMLElement * firstChild;
	struct _TBXMLElement * currentChild;
	
	struct _TBXMLElement * nextSibling;
	struct _TBXMLElement * previousSibling;
	
} TBXMLElement;

typedef struct _TBXMLElementBuffer {
	TBXMLElement * elements;
	struct _TBXMLElementBuffer * next;
	struct _TBXMLElementBuffer * previous;
} TBXMLElementBuffer;

typedef struct _TBXMLAttributeBuffer {
	TBXMLAttribute * attributes;
	struct _TBXMLAttributeBuffer * next;
	struct _TBXMLAttributeBuffer * previous;
} TBXMLAttributeBuffer;

// ================================================================================================
//  TBXML Public Interface
// ================================================================================================
class sgXML
{
	public:
		sgXML(const char *aXMLFile);
		~sgXML();
		void initWithXMLFile(const char *aXMLFile);
	
		static const char *elementName(TBXMLElement *aXMLElement);
		static const char *textForElement(TBXMLElement *aXMLElement);
	
		static const char *valueOfAttribute(const char *aName, TBXMLElement *aXMLElement);
		static const char *attributeName(TBXMLAttribute *aXMLAttribute);
		static const char *attributeValue(TBXMLAttribute *aXMLAttribute);
	
		static TBXMLElement *nextSibling(const char *aName, TBXMLElement *aXMLElement);
		static TBXMLElement *childElement(const char *aName, TBXMLElement *aParentXMLElement);
	
		TBXMLElement * rootXMLElement;
	
	private:
		void decodeData(const char *data);
		void decodeBytes();
		TBXMLElement *nextAvailableElement();
		TBXMLAttribute *nextAvailableAttribute();
	
		TBXMLElementBuffer * currentElementBuffer;
		TBXMLAttributeBuffer * currentAttributeBuffer;
		
		long currentElement;
		long currentAttribute;
		
		char * bytes;
		long bytesLength;
};

#endif
