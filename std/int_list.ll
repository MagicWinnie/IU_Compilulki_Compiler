%Integer = type { i32 }
%IntArray = type { i32*, i32 } ; { i32* data, i32 length }
%IntList = type { %IntArray }  ; Wrapping IntArray as IntList

; External malloc declaration for dynamic memory allocation
declare ptr @malloc(i32)

; Declare external printf function for output
declare i32 @printf(ptr, ...)

; Define a format string for printing integers (e.g., "%d " for each element)
@int_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1

; Declare IntArray functions
declare void @IntArray_Constructor_Integer(ptr, i32)
declare %Integer @IntArray_Length(ptr)
declare %Integer @IntArray_get_Integer(ptr, i32)
declare void @IntArray_set_Integer_Integer(ptr, i32, i32)
declare void @IntArray_print(ptr)

; Create_Default() for IntList
define void @IntList_Constructor(ptr %listPtr) {
entry:
  ; Allocate memory for the internal IntArray
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0

  ; Call IntArray_Create_Default with initial length 0
  call void @IntArray_Constructor_Integer(ptr %arrayPtr, i32 0)

  ret void
}

; Create_Default(length: Integer) for IntList
define void @IntList_Constructor_Integer(ptr %listPtr, i32 %length) {
entry:
  ; Allocate memory for the internal IntArray
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0

  ; Call IntArray_Create_Default with given length
  call void @IntArray_Constructor_Integer(ptr %arrayPtr, i32 %length)

  ret void
}

; Append(value: Integer) for IntList
define void @IntList_append_Integer(ptr %listPtr, i32 %value) {
entry:
  ; Access the internal IntArray
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0

  ; Get the current length of the IntArray
  %lengthTemp = call %Integer @IntArray_Length(ptr %arrayPtr)
  %lengthPtr = alloca %Integer
  store %Integer %lengthTemp, ptr %lengthPtr
  %lengthFieldPtr = getelementptr inbounds %Integer, ptr %lengthPtr, i32 0, i32 0
  %length = load i32, ptr %lengthFieldPtr

  ; Set the value at the next available position
  call void @IntArray_set_Integer_Integer(ptr %arrayPtr, i32 %length, i32 %value)

  ; Update the length by incrementing
  %newLength = add i32 %length, 1
  %lengthFieldPtr2 = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
  store i32 %newLength, ptr %lengthFieldPtr2

  ret void
}


; Head() -> Integer for IntList
define %Integer @IntList_head(ptr %listPtr) {
entry:
  ; Access the internal IntArray
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0

  ; Get the first element (index 0)
  %value = call %Integer @IntArray_get_Integer(ptr %arrayPtr, i32 0)

  ; Return the value
  ret %Integer %value
}

; Tail() -> IntList for IntList
define ptr @IntList_tail(ptr %listPtr) {
entry:
  ; Compute the size of IntList
  %nullPtr = inttoptr i32 0 to ptr
  %endPtr = getelementptr inbounds %IntList, ptr %nullPtr, i32 1
  %listSize = ptrtoint ptr %endPtr to i32

  ; Allocate a new IntList
  %newListPtr = call ptr @malloc(i32 %listSize)

  ; Access the internal IntArray
  %arrayPtr = getelementptr inbounds %IntList, ptr %listPtr, i32 0, i32 0

  ; Get the current length of the IntArray
  %lengthTemp = call %Integer @IntArray_Length(ptr %arrayPtr)
  %lengthPtr = alloca %Integer
  store %Integer %lengthTemp, ptr %lengthPtr
  %lengthFieldPtr = getelementptr inbounds %Integer, ptr %lengthPtr, i32 0, i32 0
  %length = load i32, ptr %lengthFieldPtr

  ; Calculate the new length for the tail (old length - 1)
  %newLength = sub i32 %length, 1

  ; Allocate a new IntArray for the tail
  %newArrayPtr = getelementptr inbounds %IntList, ptr %newListPtr, i32 0, i32 0
  call void @IntArray_Constructor_Integer(ptr %newArrayPtr, i32 %newLength)

  ; Copy elements from index 1 to the new IntArray
  %index = alloca i32, align 4
  store i32 0, ptr %index

  br label %loop

loop:
  %currentIndex = load i32, ptr %index
  %isEnd = icmp slt i32 %currentIndex, %newLength
  br i1 %isEnd, label %body, label %end

body:
  %sourceIndex = add i32 %currentIndex, 1
  %valueTemp = call %Integer @IntArray_get_Integer(ptr %arrayPtr, i32 %sourceIndex)
  %valuePtr = alloca %Integer
  store %Integer %valueTemp, ptr %valuePtr
  %valueFieldPtr = getelementptr inbounds %Integer, ptr %valuePtr, i32 0, i32 0
  %valueInt = load i32, ptr %valueFieldPtr
  call void @IntArray_set_Integer_Integer(ptr %newArrayPtr, i32 %currentIndex, i32 %valueInt)

  ; Increment index
  %nextIndex = add i32 %currentIndex, 1
  store i32 %nextIndex, ptr %index
  br label %loop

end:
  ret ptr %newListPtr
}

