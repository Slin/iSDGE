// ================================================================================================
//  sgXML.cpp, heavily modified TBXML.m
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
#include <cstring>
#include <cstdlib>
#include <cctype>

#include "sgXML.h"
#include "sgResourceManager.h"
#include "sgDebug.h"


// ================================================================================================
// Public Implementation
// ================================================================================================

sgXML::sgXML(const char *aXMLFile)
{
	rootXMLElement = NULL;
	
	currentElementBuffer = 0;
	currentAttributeBuffer = 0;
	
	currentElement = 0;
	currentAttribute = 0;		
	
	bytes = 0;
	bytesLength = 0;
	
	initWithXMLFile(aXMLFile);
}

void sgXML::initWithXMLFile(const char *aXMLFile)
{
	const char *filepath = sgResourceManager::getPath(aXMLFile);
	const char *content = sgResourceManager::getFileAsString(filepath);
	delete[] filepath;
	
	// decode data
	decodeData(content);
	delete[] content;
}

const char *sgXML::elementName(TBXMLElement *aXMLElement)
{
	if(NULL == aXMLElement->name) return NULL;
	return &aXMLElement->name[0];
}

const char *sgXML::attributeName(TBXMLAttribute *aXMLAttribute)
{
	if (NULL == aXMLAttribute->name) return NULL;
	return &aXMLAttribute->name[0];
}

const char *sgXML::attributeValue(TBXMLAttribute *aXMLAttribute)
{
	if (NULL == aXMLAttribute->value) return NULL;
	return &aXMLAttribute->value[0];
}

const char *sgXML::textForElement(TBXMLElement *aXMLElement)
{
	if (NULL == aXMLElement->text) return NULL;
	return &aXMLElement->text[0];
}

const char *sgXML::valueOfAttribute(const char *aName, TBXMLElement *aXMLElement)
{
	const char *value = NULL;
	TBXMLAttribute * attribute = aXMLElement->firstAttribute;
	while (attribute)
	{
		if (strlen(attribute->name) == strlen(aName) && memcmp(attribute->name,aName,strlen(aName)) == 0)
		{
			value = &attribute->value[0];
			break;
		}
		attribute = attribute->next;
	}
	return value;
}

TBXMLElement *sgXML::childElement(const char *aName, TBXMLElement *aParentXMLElement)
{
	TBXMLElement * xmlElement = aParentXMLElement->firstChild;
	while (xmlElement)
	{
		if (strlen(xmlElement->name) == strlen(aName) && memcmp(xmlElement->name,aName,strlen(aName)) == 0)
		{
			return xmlElement;
		}
		xmlElement = xmlElement->nextSibling;
	}
	return NULL;
}

TBXMLElement *sgXML::nextSibling(const char *aName, TBXMLElement *aXMLElement)
{
	TBXMLElement * xmlElement = aXMLElement->nextSibling;
	while (xmlElement)
	{
		if (strlen(xmlElement->name) == strlen(aName) && memcmp(xmlElement->name,aName,strlen(aName)) == 0)
		{
			return xmlElement;
		}
		xmlElement = xmlElement->nextSibling;
	}
	return NULL;
}

void sgXML::decodeData(const char *data)
{
	// copy data to byte array
	bytesLength = strlen(data);
	bytes = (char*)malloc(bytesLength);
	strcpy(bytes, data);
	
	// set null terminator at end of byte array
	bytes[bytesLength] = 0;
	
	// decode xml data
	decodeBytes();
}

void sgXML::decodeBytes()
{
	
	// -----------------------------------------------------------------------------
	// Process xml
	// -----------------------------------------------------------------------------
	
	// set elementStart pointer to the start of our xml
	char * elementStart=bytes;
	
	// set parent element to NULL
	TBXMLElement * parentXMLElement = NULL;
	
	// find next element start
	while (elementStart = strstr(elementStart,"<")) {
		
		// detect comment section
		if (strncmp(elementStart,"<!--",4) == 0) {
			elementStart = strstr(elementStart,"-->") + 3;
			continue;
		}

		// detect cdata section within element text
		int isCDATA = strncmp(elementStart,"<![CDATA[",9);
		
		// if cdata section found, skip data within cdata section and remove cdata tags
		if (isCDATA==0) {
			// find end of cdata section
			char * CDATAEnd = strstr(elementStart,"]]>");
			
			// find start of next element skipping any cdata sections within text
			char * elementEnd = CDATAEnd;
			
			// find next open tag
			elementEnd = strstr(elementEnd,"<");
			// if open tag is a cdata section
			while (strncmp(elementEnd,"<![CDATA[",9) == 0) {
				// find end of cdata section
				elementEnd = strstr(elementEnd,"]]>");
				// find next open tag
				elementEnd = strstr(elementEnd,"<");
			}
			
			// calculate length of cdata content
			long CDATALength = CDATAEnd-elementStart;
			
			// calculate total length of text
			long textLength = elementEnd-elementStart;
			
			// remove begining cdata section tag
			memcpy(elementStart, elementStart+9, CDATAEnd-elementStart-9);

			// remove ending cdata section tag
			memcpy(CDATAEnd-9, CDATAEnd+3, textLength-CDATALength-3);
			
			// blank out end of text
			memset(elementStart+textLength-12,' ',12);
			
			// set new search start position 
			elementStart = CDATAEnd-9;
			continue;
		}
		
		
		// find element end, skipping any cdata sections within attributes
		char * elementEnd = elementStart+1;		
		while (elementEnd = strpbrk(elementEnd, "<>")) {
			if (strncmp(elementEnd,"<![CDATA[",9) == 0) {
				elementEnd = strstr(elementEnd,"]]>")+3;
			} else {
				break;
			}
		}
		
		
		// null terminate element end
		if (elementEnd) *elementEnd = 0;
		
		// null terminate element start so previous element text doesnt overrun
		*elementStart = 0;
		
		// get element name start
		char * elementNameStart = elementStart+1;
		
		// ignore tags that start with ? or ! unless cdata "<![CDATA"
		if (*elementNameStart == '?' || (*elementNameStart == '!' && isCDATA != 0)) {
			elementStart = elementEnd+1;
			continue;
		}
		
		// ignore attributes/text if this is a closing element
		if (*elementNameStart == '/') {
			elementStart = elementEnd+1;
			if (parentXMLElement) {

				if (parentXMLElement->text) {
					// trim whitespace from start of text
					while (isspace(*parentXMLElement->text)) 
						parentXMLElement->text++;
					
					// trim whitespace from end of text
					char * end = parentXMLElement->text + strlen(parentXMLElement->text)-1;
					while (end > parentXMLElement->text && isspace(*end)) 
						*end--=0;
				}
				
				parentXMLElement = parentXMLElement->parentElement;
				
				// if parent element has children clear text
				if (parentXMLElement && parentXMLElement->firstChild)
					parentXMLElement->text = 0;
				
			}
			continue;
		}
		
		
		// is this element opening and closing
		bool selfClosingElement = false;
		if (*(elementEnd-1) == '/') {
			selfClosingElement = true;
		}
		
		
		// create new xmlElement struct
		TBXMLElement * xmlElement = nextAvailableElement();
		
		// set element name
		xmlElement->name = elementNameStart;
		
		// if there is a parent element
		if (parentXMLElement) {
			
			// if this is first child of parent element
			if (parentXMLElement->currentChild) {
				// set next child element in list
				parentXMLElement->currentChild->nextSibling = xmlElement;
				xmlElement->previousSibling = parentXMLElement->currentChild;
				
				parentXMLElement->currentChild = xmlElement;
				
				
			} else {
				// set first child element
				parentXMLElement->currentChild = xmlElement;
				parentXMLElement->firstChild = xmlElement;
			}
			
			xmlElement->parentElement = parentXMLElement;
		}
		
		
		// in the following xml the ">" is replaced with \0 by elementEnd. 
		// element may contain no atributes and would return NULL while looking for element name end
		// <tile> 
		// find end of element name
		char * elementNameEnd = strpbrk(xmlElement->name," /");
		
		
		// if end was found check for attributes
		if (elementNameEnd) {
			
			// null terminate end of elemenet name
			*elementNameEnd = 0;
			
			char * chr = elementNameEnd;
			char * name = NULL;
			char * value = NULL;
			char * CDATAStart = NULL;
			char * CDATAEnd = NULL;
			TBXMLAttribute * lastXMLAttribute = NULL;
			TBXMLAttribute * xmlAttribute = NULL;
			bool singleQuote = false;
			
			int mode = TBXML_ATTRIBUTE_NAME_START;
			
			// loop through all characters within element
			while (chr++ < elementEnd) {
				
				switch (mode) {
					// look for start of attribute name
					case TBXML_ATTRIBUTE_NAME_START:
						if (isspace(*chr)) continue;
						name = chr;
						mode = TBXML_ATTRIBUTE_NAME_END;
						break;
					// look for end of attribute name
					case TBXML_ATTRIBUTE_NAME_END:
						if (isspace(*chr) || *chr == '=') {
							*chr = 0;
							mode = TBXML_ATTRIBUTE_VALUE_START;
						}
						break;
					// look for start of attribute value
					case TBXML_ATTRIBUTE_VALUE_START:
						if (isspace(*chr)) continue;
						if (*chr == '"' || *chr == '\'') {
							value = chr+1;
							mode = TBXML_ATTRIBUTE_VALUE_END;
							if (*chr == '\'') 
								singleQuote = true;
							else
								singleQuote = false;
						}
						break;
					// look for end of attribute value
					case TBXML_ATTRIBUTE_VALUE_END:
						if (*chr == '<' && strncmp(chr, "<![CDATA[", 9) == 0) {
							mode = TBXML_ATTRIBUTE_CDATA_END;
						}else if ((*chr == '"' && singleQuote == false) || (*chr == '\'' && singleQuote == true)) {
							*chr = 0;
							
							// remove cdata section tags
							while (CDATAStart = strstr(value, "<![CDATA[")) {
								
								// remove begin cdata tag
								memcpy(CDATAStart, CDATAStart+9, strlen(CDATAStart)-8);
								
								// search for end cdata
								CDATAEnd = strstr(CDATAStart,"]]>");
								
								// remove end cdata tag
								memcpy(CDATAEnd, CDATAEnd+3, strlen(CDATAEnd)-2);
							}
							
							
							// create new attribute
							xmlAttribute = nextAvailableAttribute();
							
							// if this is the first attribute found, set pointer to this attribute on element
							if (!xmlElement->firstAttribute) xmlElement->firstAttribute = xmlAttribute;
							// if previous attribute found, link this attribute to previous one
							if (lastXMLAttribute) lastXMLAttribute->next = xmlAttribute;
							// set last attribute to this attribute
							lastXMLAttribute = xmlAttribute;

							// set attribute name & value
							xmlAttribute->name = name;
							xmlAttribute->value = value;
							
							// clear name and value pointers
							name = NULL;
							value = NULL;
							
							// start looking for next attribute
							mode = TBXML_ATTRIBUTE_NAME_START;
						}
						break;
						// look for end of cdata
					case TBXML_ATTRIBUTE_CDATA_END:
						if (*chr == ']') {
							if (strncmp(chr, "]]>", 3) == 0) {
								mode = TBXML_ATTRIBUTE_VALUE_END;
							}
						}
						break;						
					default:
						break;
				}
			}
		}
		
		// if tag is not self closing, set parent to current element
		if (!selfClosingElement) {
			// set text on element to element end+1
			if (*(elementEnd+1) != '>')
				xmlElement->text = elementEnd+1;
			
			parentXMLElement = xmlElement;
		}
		
		// start looking for next element after end of current element
		elementStart = elementEnd+1;
	}
}

// Deallocate used memory
sgXML::~sgXML()
{
	if (bytes)
	{
		free(bytes);
		bytes = NULL;
	}
	
	while (currentElementBuffer)
	{
		if (currentElementBuffer->elements)
			free(currentElementBuffer->elements);
		
		if (currentElementBuffer->previous)
		{
			currentElementBuffer = currentElementBuffer->previous;
			free(currentElementBuffer->next);
		} else
		{
			free(currentElementBuffer);
			currentElementBuffer = 0;
		}
	}
	
	while (currentAttributeBuffer)
	{
		if (currentAttributeBuffer->attributes)
			free(currentAttributeBuffer->attributes);
		
		if (currentAttributeBuffer->previous)
		{
			currentAttributeBuffer = currentAttributeBuffer->previous;
			free(currentAttributeBuffer->next);
		} else
		{
			free(currentAttributeBuffer);
			currentAttributeBuffer = 0;
		}
	}
}

TBXMLElement *sgXML::nextAvailableElement()
{
	currentElement++;
	
	if (!currentElementBuffer) {
		currentElementBuffer = (TBXMLElementBuffer*)calloc(1, sizeof(TBXMLElementBuffer));
		currentElementBuffer->elements = (TBXMLElement*)calloc(1,sizeof(TBXMLElement)*MAX_ELEMENTS);
		currentElement = 0;
		rootXMLElement = &currentElementBuffer->elements[currentElement];
	} else if (currentElement > MAX_ELEMENTS) {
		currentElementBuffer->next = (TBXMLElementBuffer*)calloc(1, sizeof(TBXMLElementBuffer));
		currentElementBuffer->next->previous = currentElementBuffer;
		currentElementBuffer = currentElementBuffer->next;
		currentElementBuffer->elements = (TBXMLElement*)calloc(1,sizeof(TBXMLElement)*MAX_ELEMENTS);
		currentElement = 0;
	}
	
	return &currentElementBuffer->elements[currentElement];
}

TBXMLAttribute *sgXML::nextAvailableAttribute()
{
	currentAttribute++;
	
	if (!currentAttributeBuffer) {
		currentAttributeBuffer = (TBXMLAttributeBuffer*)calloc(1, sizeof(TBXMLAttributeBuffer));
		currentAttributeBuffer->attributes = (TBXMLAttribute*)calloc(MAX_ATTRIBUTES,sizeof(TBXMLAttribute));
		currentAttribute = 0;
	} else if (currentAttribute > MAX_ATTRIBUTES) {
		currentAttributeBuffer->next = (TBXMLAttributeBuffer*)calloc(1, sizeof(TBXMLAttributeBuffer));
		currentAttributeBuffer->next->previous = currentAttributeBuffer;
		currentAttributeBuffer = currentAttributeBuffer->next;
		currentAttributeBuffer->attributes = (TBXMLAttribute*)calloc(MAX_ATTRIBUTES,sizeof(TBXMLAttribute));
		currentAttribute = 0;
	}
	
	return &currentAttributeBuffer->attributes[currentAttribute];
}
