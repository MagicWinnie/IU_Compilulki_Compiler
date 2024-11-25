%Boolean = type { i1 }
%BoolArray = type { i1*, i32 } ; { i1* data, i32 length }

%Integer = type { i32 }

; External malloc declaration for dynamic memory allocation
declare ptr @malloc(i32)

; Constructor for BoolArray (this(l: Integer))
define void @BoolArray_Constructor_Integer(ptr %arrayPtr, i32 %length) {
entry:
  ; Allocate memory for 'length' elements of i1 (1 byte per element)
  %elementSize = mul i32 %length, 1
  %dataPtr = call ptr @malloc(i32 %elementSize)

  ; Store the data pointer in the BoolArray structure
  %dataFieldPtr = getelementptr inbounds %BoolArray, ptr %arrayPtr, i32 0, i32 0
  store ptr %dataPtr, ptr %dataFieldPtr

  ; Store the length in the BoolArray structure
  %lengthFieldPtr = getelementptr inbounds %BoolArray, ptr %arrayPtr, i32 0, i32 1
  store i32 %length, ptr %lengthFieldPtr

  ret void
}

; Method to get the length of the BoolArray (Length)
define %Integer @BoolArray_Length(ptr %arrayPtr) {
entry:
  ; Get the length field from the BoolArray structure
  %lengthFieldPtr = getelementptr inbounds %BoolArray, ptr %arrayPtr, i32 0, i32 1
  %length = load i32, ptr %lengthFieldPtr

  ; Allocate memory for a new %Integer
  %newInteger = alloca %Integer, align 4

  ; Store the length value in the new %Integer
  %integerFieldPtr = getelementptr inbounds %Integer, ptr %newInteger, i32 0, i32 0
  store i32 %length, ptr %integerFieldPtr, align 4

  ; Return the new %Integer
  %returnValue = load %Integer, ptr %newInteger
  ret %Integer %returnValue
}

; Method to get an element from the BoolArray (get(i: Integer) : T)
define %Boolean @BoolArray_get_Integer(ptr %arrayPtr, i32 %index) {
entry:
  ; Get the data pointer from the BoolArray structure
  %dataFieldPtr = getelementptr inbounds %BoolArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr

  ; Calculate the pointer to the specific index
  %elementPtr = getelementptr inbounds i1, ptr %dataPtr, i32 %index

  ; Load the value from the specified index
  %value = load i1, ptr %elementPtr

  ; Create a %Boolean structure to hold the value
  %booleanValue = alloca %Boolean
  %booleanFieldPtr = getelementptr inbounds %Boolean, ptr %booleanValue, i32 0, i32 0
  store i1 %value, ptr %booleanFieldPtr

  ; Load the %Boolean structure to return it
  %returnValue = load %Boolean, ptr %booleanValue
  ret %Boolean %returnValue
}

; Method to set an element in the BoolArray (set(i: Integer, v: T))
define void @BoolArray_set_Integer_Boolean(ptr %arrayPtr, i32 %index, i1 %value) {
entry:
  ; Get the data pointer from the BoolArray structure
  %dataFieldPtr = getelementptr inbounds %BoolArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr

  ; Calculate the pointer to the specific index
  %elementPtr = getelementptr inbounds i1, ptr %dataPtr, i32 %index

  ; Store the value at the specified index
  store i1 %value, ptr %elementPtr
  ret void
}

; Declare external printf function for output
declare i32 @printf(ptr, ...)

; Define a format string for printing booleans (e.g., "%d " for each element, but we'll print 1 for true and 0 for false)
@bool_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1

; Method to print the contents of the BoolArray
define void @BoolArray_print(ptr %arrayPtr) {
entry:
  ; Get the data pointer from the BoolArray structure
  %dataFieldPtr = getelementptr inbounds %BoolArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr

  ; Get the length of the BoolArray
  %lengthFieldPtr = getelementptr inbounds %BoolArray, ptr %arrayPtr, i32 0, i32 1
  %length = load i32, ptr %lengthFieldPtr

  ; Initialize loop index
  %index = alloca i32, align 4
  store i32 0, ptr %index

  ; Loop label
  br label %loop

loop: ; Loop header
  %currentIndex = load i32, ptr %index
  %isEnd = icmp slt i32 %currentIndex, %length
  br i1 %isEnd, label %body, label %end

body: ; Loop body
  ; Calculate the pointer to the current element
  %elementPtr = getelementptr inbounds i1, ptr %dataPtr, i32 %currentIndex
  %elementValue = load i1, ptr %elementPtr

  ; Print the current element using printf (print 1 for true, 0 for false)
  %1 = call i32 (ptr, ...) @printf(ptr @bool_fmt, i1 %elementValue)

  ; Increment the index
  %nextIndex = add i32 %currentIndex, 1
  store i32 %nextIndex, ptr %index
  br label %loop

end: ; Loop end
  ret void
}
