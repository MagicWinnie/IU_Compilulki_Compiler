; ModuleID = 'RealArray'
source_filename = "real_array"

%Real = type { double }
%Integer = type { i32 }
%RealArray = type { double*, i32 } ; { double* data, i32 length }

; External malloc declaration for dynamic memory allocation
declare ptr @malloc(i32)

; Constructor for RealArray (this(l: Integer))
define void @RealArray_Constructor_Integer(ptr %arrayPtr, i32 %length) {
entry:
  ; Allocate memory for 'length' elements of double (8 bytes per element)
  %elementSize = mul i32 %length, 8
  %dataPtr = call ptr @malloc(i32 %elementSize)

  ; Store the data pointer in the RealArray structure
  %dataFieldPtr = getelementptr inbounds %RealArray, ptr %arrayPtr, i32 0, i32 0
  store ptr %dataPtr, ptr %dataFieldPtr

  ; Store the length in the RealArray structure
  %lengthFieldPtr = getelementptr inbounds %RealArray, ptr %arrayPtr, i32 0, i32 1
  store i32 %length, ptr %lengthFieldPtr

  ret void
}

; Method to get the length of the RealArray (Length)
define %Integer @RealArray_Length(ptr %arrayPtr) {
entry:
  ; Get the length field from the RealArray structure
  %lengthFieldPtr = getelementptr inbounds %RealArray, ptr %arrayPtr, i32 0, i32 1
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

; Method to get an element from the RealArray (get(i: Integer) : T)
define %Real @RealArray_get_Integer(ptr %arrayPtr, i32 %index) {
entry:
  ; Get the data pointer from the RealArray structure
  %dataFieldPtr = getelementptr inbounds %RealArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr

  ; Calculate the pointer to the specific index
  %elementPtr = getelementptr inbounds double, ptr %dataPtr, i32 %index

  ; Load the value from the specified index
  %value = load double, ptr %elementPtr

  ; Create a %Real structure to hold the value
  %realValue = alloca %Real
  %realFieldPtr = getelementptr inbounds %Real, ptr %realValue, i32 0, i32 0
  store double %value, ptr %realFieldPtr

  ; Load the %Real structure to return it
  %returnValue = load %Real, ptr %realValue
  ret %Real %returnValue
}

; Method to set an element in the RealArray (set(i: Integer, v: T))
define void @RealArray_set_Integer_Real(ptr %arrayPtr, i32 %index, double %value) {
entry:
  ; Get the data pointer from the RealArray structure
  %dataFieldPtr = getelementptr inbounds %RealArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr

  ; Calculate the pointer to the specific index
  %elementPtr = getelementptr inbounds double, ptr %dataPtr, i32 %index

  ; Store the value at the specified index
  store double %value, ptr %elementPtr
  ret void
}

; Declare external printf function for output
declare i32 @printf(ptr, ...)

; Define a format string for printing doubles (e.g., "%f " for each element)
@real_fmt = private unnamed_addr constant [4 x i8] c"%f \00", align 1

; Method to print the contents of the RealArray
define void @RealArray_print(ptr %arrayPtr) {
entry:
  ; Get the data pointer from the RealArray structure
  %dataFieldPtr = getelementptr inbounds %RealArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr

  ; Get the length of the RealArray
  %lengthFieldPtr = getelementptr inbounds %RealArray, ptr %arrayPtr, i32 0, i32 1
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
  %elementPtr = getelementptr inbounds double, ptr %dataPtr, i32 %currentIndex
  %elementValue = load double, ptr %elementPtr

  ; Print the current element using printf
  %1 = call i32 (ptr, ...) @printf(ptr @real_fmt, double %elementValue)

  ; Increment the index
  %nextIndex = add i32 %currentIndex, 1
  store i32 %nextIndex, ptr %index
  br label %loop

end: ; Loop end
  ret void
}