%Integer = type { i32 }
%IntArray = type { i32*, i32 } ; { i32* data, i32 length }

; External malloc declaration for dynamic memory allocation
declare ptr @malloc(i32)

; Constructor for IntArray (this(l: Integer))
define void @IntArray_Constructor_Integer(ptr %arrayPtr, i32 %length) {
entry:
  ; Allocate memory for 'length' elements of i32 (4 bytes per element)
  %elementSize = mul i32 %length, 4
  %dataPtr = call ptr @malloc(i32 %elementSize)

  ; Store the data pointer in the IntArray structure
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  store ptr %dataPtr, ptr %dataFieldPtr

  ; Store the length in the IntArray structure
  %lengthFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
  store i32 %length, ptr %lengthFieldPtr

  ret void
}

; Method to get the length of the IntArray (Length)
define %Integer @IntArray_Length(ptr %arrayPtr) {
entry:
  ; Get the length field from the IntArray structure
  %lengthFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
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

; Method to get an element from the IntArray (get(i: Integer) : T)
define %Integer @IntArray_get_Integer(ptr %arrayPtr, i32 %index) {
entry:
  ; Get the data pointer from the IntArray structure
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr

  ; Calculate the pointer to the specific index
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i32 %index

  ; Load the value from the specified index
  %value = load i32, ptr %elementPtr

  ; Create a %Integer structure to hold the value
  %integerValue = alloca %Integer
  %integerFieldPtr = getelementptr inbounds %Integer, ptr %integerValue, i32 0, i32 0
  store i32 %value, ptr %integerFieldPtr

  ; Load the %Integer structure to return it
  %returnValue = load %Integer, ptr %integerValue
  ret %Integer %returnValue
}

; Method to set an element in the IntArray (set(i: Integer, v: T))
define void @IntArray_set_Integer_Integer(ptr %arrayPtr, i32 %index, i32 %value) {
entry:
  ; Get the data pointer from the IntArray structure
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr

  ; Calculate the pointer to the specific index
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i32 %index

  ; Store the value at the specified index
  store i32 %value, ptr %elementPtr
  ret void
}
; Declare external printf function for output
declare i32 @printf(ptr, ...)

; Define a format string for printing integers (e.g., "%d " for each element)
@int_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1

; Method to print the contents of the IntArray
define void @IntArray_print(ptr %arrayPtr) {
entry:
  ; Get the data pointer from the IntArray structure
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr

  ; Get the length of the IntArray
  %lengthFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
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
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i32 %currentIndex
  %elementValue = load i32, ptr %elementPtr

  ; Print the current element using printf
  %1 = call i32 (ptr, ...) @printf(ptr @int_fmt, i32 %elementValue)

  ; Increment the index
  %nextIndex = add i32 %currentIndex, 1
  store i32 %nextIndex, ptr %index
  br label %loop

end: ; Loop end
  ret void
}