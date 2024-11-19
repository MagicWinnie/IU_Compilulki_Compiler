%Real = type { double }
%RealArray = type { double*, i32 } ; { double* data, i32 length }
%RealList = type { %RealArray }    ; Wrapping RealArray as RealList

; External malloc declaration for dynamic memory allocation
declare ptr @malloc(i32)

; Declare external printf function for output
declare i32 @printf(ptr, ...)

; Define a format string for printing reals (e.g., "%f " for each element)
@real_fmt = private unnamed_addr constant [4 x i8] c"%f \00", align 1

; Declare RealArray functions
declare void @RealArray_Constructor_Real(ptr, i32)
declare %Real @RealArray_Length(ptr)
declare %Real @RealArray_get_Real(ptr, i32)
declare void @RealArray_set_Real_Real(ptr, i32, double)
declare void @RealArray_print(ptr)

; Create_Default() for RealList
define void @RealList_Constructor(ptr %listPtr) {
entry:
  ; Allocate memory for the internal RealArray
  %arrayPtr = getelementptr inbounds %RealList, ptr %listPtr, i32 0, i32 0

  ; Call RealArray_Create_Default with initial length 0
  call void @RealArray_Constructor_Real(ptr %arrayPtr, i32 0)

  ret void
}

; Create_Default(length: Integer) for RealList
define void @RealList_Constructor_Integer(ptr %listPtr, i32 %length) {
entry:
  ; Allocate memory for the internal RealArray
  %arrayPtr = getelementptr inbounds %RealList, ptr %listPtr, i32 0, i32 0

  ; Call RealArray_Create_Default with given length
  call void @RealArray_Constructor_Real(ptr %arrayPtr, i32 %length)

  ret void
}

; Append(value: Real) for RealList
define void @RealList_append_Real(ptr %listPtr, double %value) {
entry:
  ; Access the internal RealArray
  %arrayPtr = getelementptr inbounds %RealList, ptr %listPtr, i32 0, i32 0

  ; Get the current length of the RealArray
  %lengthTemp = call %Real @RealArray_Length(ptr %arrayPtr)
  %lengthPtr = alloca %Real
  store %Real %lengthTemp, ptr %lengthPtr
  %lengthFieldPtr = getelementptr inbounds %Real, ptr %lengthPtr, i32 0, i32 0
  %length = load i32, ptr %lengthFieldPtr

  ; Set the value at the next available position
  call void @RealArray_set_Real_Real(ptr %arrayPtr, i32 %length, double %value)

  ; Update the length by incrementing
  %newLength = add i32 %length, 1
  %lengthFieldPtr2 = getelementptr inbounds %RealArray, ptr %arrayPtr, i32 0, i32 1
  store i32 %newLength, ptr %lengthFieldPtr2

  ret void
}

; Head() -> Real for RealList
define %Real @RealList_head(ptr %listPtr) {
entry:
  ; Access the internal RealArray
  %arrayPtr = getelementptr inbounds %RealList, ptr %listPtr, i32 0, i32 0

  ; Get the first element (index 0)
  %value = call %Real @RealArray_get_Real(ptr %arrayPtr, i32 0)

  ; Return the value
  ret %Real %value
}

; Tail() -> RealList for RealList
define ptr @RealList_tail(ptr %listPtr) {
entry:
  ; Compute the size of RealList
  %nullPtr = inttoptr i32 0 to ptr
  %endPtr = getelementptr inbounds %RealList, ptr %nullPtr, i32 1
  %listSize = ptrtoint ptr %endPtr to i32

  ; Allocate a new RealList
  %newListPtr = call ptr @malloc(i32 %listSize)

  ; Access the internal RealArray
  %arrayPtr = getelementptr inbounds %RealList, ptr %listPtr, i32 0, i32 0

  ; Get the current length of the RealArray
  %lengthTemp = call %Real @RealArray_Length(ptr %arrayPtr)
  %lengthPtr = alloca %Real
  store %Real %lengthTemp, ptr %lengthPtr
  %lengthFieldPtr = getelementptr inbounds %Real, ptr %lengthPtr, i32 0, i32 0
  %length = load i32, ptr %lengthFieldPtr

  ; Calculate the new length for the tail (old length - 1)
  %newLength = sub i32 %length, 1

  ; Allocate a new RealArray for the tail
  %newArrayPtr = getelementptr inbounds %RealList, ptr %newListPtr, i32 0, i32 0
  call void @RealArray_Constructor_Real(ptr %newArrayPtr, i32 %newLength)

  ; Copy elements from index 1 to the new RealArray
  %index = alloca i32, align 4
  store i32 0, ptr %index

  br label %loop

loop:
  %currentIndex = load i32, ptr %index
  %isEnd = icmp slt i32 %currentIndex, %newLength
  br i1 %isEnd, label %body, label %end

body:
  %sourceIndex = add i32 %currentIndex, 1
  %valueTemp = call %Real @RealArray_get_Real(ptr %arrayPtr, i32 %sourceIndex)
  %valuePtr = alloca %Real
  store %Real %valueTemp, ptr %valuePtr
  %valueFieldPtr = getelementptr inbounds %Real, ptr %valuePtr, i32 0, i32 0
  %valueReal = load double, ptr %valueFieldPtr
  call void @RealArray_set_Real_Real(ptr %newArrayPtr, i32 %currentIndex, double %valueReal)

  ; Increment index
  %nextIndex = add i32 %currentIndex, 1
  store i32 %nextIndex, ptr %index
  br label %loop

end:
  ret ptr %newListPtr
}
