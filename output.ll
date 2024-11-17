; ModuleID = 'compilul'ki'
source_filename = "compilul'ki"

%Boolean = type { i1 }
%Integer = type { i32 }
%Real = type { double }
%IntArray = type { ptr, i32 }
%A = type { %Integer, %Integer }
%Main = type {}

@fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"true\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"false\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt_scan = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@error_msg = private unnamed_addr constant [33 x i8] c"Invalid input. Exiting program.\0A\00", align 1
@fmt.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@int_fmt = private unnamed_addr constant [4 x i8] c"%d \00", align 1

define void @Boolean_Constructor_Boolean(ptr %0, i1 %1) {
entry:
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  store i1 %1, ptr %boolFieldPtr, align 1
  ret void
}

define void @Boolean_print(ptr %0) {
entry:
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %0, i32 0, i32 0
  %loadBool = load i1, ptr %boolFieldPtr, align 1
  br i1 %loadBool, label %print_true, label %print_false

print_true:                                       ; preds = %entry
  %true_ptr = getelementptr inbounds [5 x i8], ptr @true_str, i32 0, i32 0
  %1 = call i32 (ptr, ...) @printf(ptr @fmt, ptr %true_ptr)
  ret void

print_false:                                      ; preds = %entry
  %false_ptr = getelementptr inbounds [6 x i8], ptr @false_str, i32 0, i32 0
  %2 = call i32 (ptr, ...) @printf(ptr @fmt, ptr %false_ptr)
  ret void
}

declare i32 @printf(ptr, ...)

define void @Integer_Constructor_Integer(ptr %0, i32 %1) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 %1, ptr %intFieldPtr, align 4
  ret void
}

define void @Integer_Constructor(ptr %0) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 0, ptr %intFieldPtr, align 4
  ret void
}

define i32 @Integer_get(ptr %integerPtr) {
entry:
  %fieldPtr = getelementptr inbounds %Integer, ptr %integerPtr, i32 0, i32 0
  %value = load i32, ptr %fieldPtr, align 4
  ret i32 %value
}

define %Integer @Integer_Mult_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = mul i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Integer @Integer_Plus_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = add i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Integer @Integer_Minus_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = sub i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Integer @Integer_Div_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = sdiv i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Integer @Integer_Rem_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = srem i32 %selfValue, %otherValue
  %returnValue = insertvalue %Integer undef, i32 %result, 0
  ret %Integer %returnValue
}

define %Boolean @Integer_Less_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = icmp slt i32 %selfValue, %otherValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Greater_Integer(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = icmp sgt i32 %selfValue, %otherValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Constructor_Boolean(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define void @Integer_print(ptr %0) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  %loadInt = load i32, ptr %intFieldPtr, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @fmt.1, i32 %loadInt)
  ret void
}

define void @Integer_scan(ptr %0) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  %scanResult = call i32 (ptr, ...) @scanf(ptr @fmt_scan, ptr %intFieldPtr)
  %ifcond = icmp eq i32 %scanResult, 1
  br i1 %ifcond, label %validInput, label %invalidInput

validInput:                                       ; preds = %entry
  ret void

invalidInput:                                     ; preds = %entry
  %1 = call i32 (ptr, ...) @printf(ptr @error_msg)
  call void @exit(i32 1)
  ret void
}

declare i32 @scanf(ptr, ...)

declare void @exit(i32)

define void @Real_Create_Default(ptr %0, double %1) {
entry:
  %doubleFieldPtr = getelementptr inbounds %Real, ptr %0, i32 0, i32 0
  store double %1, ptr %doubleFieldPtr, align 8
  ret void
}

define void @Real_print(ptr %0) {
entry:
  %doubleFieldPtr = getelementptr inbounds %Real, ptr %0, i32 0, i32 0
  %loadDouble = load double, ptr %doubleFieldPtr, align 8
  %1 = call i32 (ptr, ...) @printf(ptr @fmt.2, double %loadDouble)
  ret void
}

define void @IntArray_Create_Default(ptr %arrayPtr, i32 %length) {
entry:
  %elementSize = mul i32 %length, 4
  %dataPtr = call ptr @malloc(i32 %elementSize)
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  store ptr %dataPtr, ptr %dataFieldPtr, align 8
  %lengthFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
  store i32 %length, ptr %lengthFieldPtr, align 4
  ret void
}

declare ptr @malloc(i32)

define %Integer @IntArray_Length(ptr %arrayPtr) {
entry:
  %lengthFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
  %length = load i32, ptr %lengthFieldPtr, align 4
  %newInteger = alloca %Integer, align 4
  %integerFieldPtr = getelementptr inbounds %Integer, ptr %newInteger, i32 0, i32 0
  store i32 %length, ptr %integerFieldPtr, align 4
  %returnValue = load %Integer, ptr %newInteger, align 4
  ret %Integer %returnValue
}

define %Integer @IntArray_get_Integer(ptr %arrayPtr, i32 %index) {
entry:
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr, align 8
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i32 %index
  %value = load i32, ptr %elementPtr, align 4
  %integerValue = alloca %Integer, align 8
  %integerFieldPtr = getelementptr inbounds %Integer, ptr %integerValue, i32 0, i32 0
  store i32 %value, ptr %integerFieldPtr, align 4
  %returnValue = load %Integer, ptr %integerValue, align 4
  ret %Integer %returnValue
}

define void @IntArray_set_Integer_Integer(ptr %arrayPtr, i32 %index, i32 %value) {
entry:
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr, align 8
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i32 %index
  store i32 %value, ptr %elementPtr, align 4
  ret void
}

define void @IntArray_print(ptr %arrayPtr) {
entry:
  %dataFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 0
  %dataPtr = load ptr, ptr %dataFieldPtr, align 8
  %lengthFieldPtr = getelementptr inbounds %IntArray, ptr %arrayPtr, i32 0, i32 1
  %length = load i32, ptr %lengthFieldPtr, align 4
  %index = alloca i32, align 4
  store i32 0, ptr %index, align 4
  br label %loop

loop:                                             ; preds = %body, %entry
  %currentIndex = load i32, ptr %index, align 4
  %isEnd = icmp slt i32 %currentIndex, %length
  br i1 %isEnd, label %body, label %end

body:                                             ; preds = %loop
  %elementPtr = getelementptr inbounds i32, ptr %dataPtr, i32 %currentIndex
  %elementValue = load i32, ptr %elementPtr, align 4
  %0 = call i32 (ptr, ...) @printf(ptr @int_fmt, i32 %elementValue)
  %nextIndex = add i32 %currentIndex, 1
  store i32 %nextIndex, ptr %index, align 4
  br label %loop

end:                                              ; preds = %loop
  ret void
}

define void @A_Init(ptr %0) {
entry:
  %load_g = getelementptr inbounds %A, ptr %0, i32 0, i32 1
  %mallocCall1 = call ptr @malloc(i64 4)
  call void @Integer_Constructor_Integer(ptr %mallocCall1, i32 20)
  store ptr %mallocCall1, ptr %load_g, align 8
  %load_q = getelementptr inbounds %A, ptr %0, i32 0, i32 0
  %mallocCall = call ptr @malloc(i64 4)
  call void @Integer_Constructor_Integer(ptr %mallocCall, i32 10)
  store ptr %mallocCall, ptr %load_q, align 8
  ret void
}

define %Integer @A_getA(ptr %this) {
entry:
  %q = getelementptr inbounds %A, ptr %this, i32 0, i32 0
  %q1 = load ptr, ptr %q, align 8
  %returnVal = load %Integer, ptr %q1, align 4
  ret %Integer %returnVal
}

define void @A_Constructor(ptr %0) {
entry:
  call void @A_Init(ptr %0)
  ret void
}

define void @Main_Init(ptr %0) {
entry:
  ret void
}

define void @Main_Constructor(ptr %this) {
entry:
  call void @Main_Init(ptr %this)
  %mallocCall = call ptr @malloc(i64 8)
  call void @A_Constructor(ptr %mallocCall)
  %call_getA = call %Integer @A_getA(ptr %mallocCall)
  %alloca_return_val = alloca %Integer, align 8
  store %Integer %call_getA, ptr %alloca_return_val, align 4
  call void @Integer_print(ptr %alloca_return_val)
  ret void
}

define i32 @main() {
entry:
  %main_object = alloca %Main, align 8
  call void @Main_Constructor(ptr %main_object)
  ret i32 0
}
