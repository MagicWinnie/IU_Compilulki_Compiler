%Boolean = type { i1 }
%BoolArray = type { i1*, i32 }  ; { i1* data, i32 length }
%BoolList = type { %BoolArray } ; Wrapping BoolArray as BoolList

; External malloc declaration for dynamic memory allocation
declare ptr @malloc(i32)

; Declare external printf function for output
declare i32 @printf(ptr, ...)

; Define a format string for printing booleans (e.g., "%d " for 0/1 representation of i1)
@bool_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1

; Declare BoolArray functions
declare void @BoolArray_Constructor_Boolean(ptr, i32)
declare %Boolean @BoolArray_Length(ptr)
declare %Boolean @BoolArray_get_Boolean(ptr, i32)
declare void @BoolArray_set_Boolean_Boolean(ptr, i32, i1)
declare void @BoolArray_print(ptr)

; Create_Default() for BoolList
define void @BoolList_Constructor(ptr %listPtr) {
entry:
  ; Allocate memory for the internal BoolArray
  %arrayPtr = getelementptr inbounds %BoolList, ptr %listPtr, i32 0, i32 0

  ; Call BoolArray_Constructor_Boolean with initial length 0
  call void @BoolArray_Constructor_Boolean(ptr %arrayPtr, i32 0)

  ret void
}

; Create_Default(length: Integer) for BoolList
define void @BoolList_Constructor_Integer(ptr %listPtr, i32 %length) {
entry:
  ; Allocate memory for the internal BoolArray
  %arrayPtr = getelementptr inbounds %BoolList, ptr %listPtr, i32 0, i32 0

  ; Call BoolArray_Constructor_Boolean with the given length
  call void @BoolArray_Constructor_Boolean(ptr %arrayPtr, i32 %length)

  ret void
}

; Append(value: Boolean) for BoolList
define void @BoolList_append_Boolean(ptr %listPtr, i1 %value) {
entry:
  ; Access the internal BoolArray
  %arrayPtr = getelementptr inbounds %BoolList, ptr %listPtr, i32 0, i32 0

  ; Get the current length of the BoolArray
  %lengthTemp = call %Boolean @BoolArray_Length(ptr %arrayPtr)
  %lengthPtr = alloca %Boolean
  store %Boolean %lengthTemp, ptr %lengthPtr
  %lengthFieldPtr = getelementptr inbounds %Boolean, ptr %lengthPtr, i32 0, i32 0
  %length = load i32, ptr %lengthFieldPtr

  ; Set the value at the next available position
  call void @BoolArray_set_Boolean_Boolean(ptr %arrayPtr, i32 %length, i1 %value)

  ; Update the length by incrementing
  %newLength = add i32 %length, 1
  %lengthFieldPtr2 = getelementptr inbounds %BoolArray, ptr %arrayPtr, i32 0, i32 1
  store i32 %newLength, ptr %lengthFieldPtr2

  ret void
}

; Head() -> Boolean for BoolList
define %Boolean @BoolList_head(ptr %listPtr) {
entry:
  ; Access the internal BoolArray
  %arrayPtr = getelementptr inbounds %BoolList, ptr %listPtr, i32 0, i32 0

  ; Get the first element (index 0)
  %value = call %Boolean @BoolArray_get_Boolean(ptr %arrayPtr, i32 0)

  ; Return the value
  ret %Boolean %value
}

; Tail() -> BoolList for BoolList
define ptr @BoolList_tail(ptr %listPtr) {
entry:
  ; Compute the size of BoolList
  %nullPtr = inttoptr i32 0 to ptr
  %endPtr = getelementptr inbounds %BoolList, ptr %nullPtr, i32 1
  %listSize = ptrtoint ptr %endPtr to i32

  ; Allocate a new BoolList
  %newListPtr = call ptr @malloc(i32 %listSize)

  ; Access the internal BoolArray
  %arrayPtr = getelementptr inbounds %BoolList, ptr %listPtr, i32 0, i32 0

  ; Get the current length of the BoolArray
  %lengthTemp = call %Boolean @BoolArray_Length(ptr %arrayPtr)
  %lengthPtr = alloca %Boolean
  store %Boolean %lengthTemp, ptr %lengthPtr
  %lengthFieldPtr = getelementptr inbounds %Boolean, ptr %lengthPtr, i32 0, i32 0
  %length = load i32, ptr %lengthFieldPtr

  ; Calculate the new length for the tail (old length - 1)
  %newLength = sub i32 %length, 1

  ; Allocate a new BoolArray for the tail
  %newArrayPtr = getelementptr inbounds %BoolList, ptr %newListPtr, i32 0, i32 0
  call void @BoolArray_Constructor_Boolean(ptr %newArrayPtr, i32 %newLength)

  ; Copy elements from index 1 to the new BoolArray
  %index = alloca i32, align 4
  store i32 0, ptr %index

  br label %loop

loop:
  %currentIndex = load i32, ptr %index
  %isEnd = icmp slt i32 %currentIndex, %newLength
  br i1 %isEnd, label %body, label %end

body:
  %sourceIndex = add i32 %currentIndex, 1
  %valueTemp = call %Boolean @BoolArray_get_Boolean(ptr %arrayPtr, i32 %sourceIndex)
  %valuePtr = alloca %Boolean
  store %Boolean %valueTemp, ptr %valuePtr
  %valueFieldPtr = getelementptr inbounds %Boolean, ptr %valuePtr, i32 0, i32 0
  %valueBool = load i1, ptr %valueFieldPtr
  call void @BoolArray_set_Boolean_Boolean(ptr %newArrayPtr, i32 %currentIndex, i1 %valueBool)

  ; Increment index
  %nextIndex = add i32 %currentIndex, 1
  store i32 %nextIndex, ptr %index
  br label %loop

end:
  ret ptr %newListPtr
}
