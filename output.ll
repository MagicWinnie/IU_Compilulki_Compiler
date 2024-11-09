; ModuleID = 'compilul'ki'
source_filename = "compilul'ki"

%Boolean = type { i1 }
%Integer = type { i32 }
%Real = type { double }
%Main = type {}

@fmt = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"true\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"false\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt_scan = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@error_msg = private unnamed_addr constant [33 x i8] c"Invalid input. Exiting program.\0A\00", align 1
@fmt.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define void @Boolean_Create_Default(ptr %0, i1 %1) {
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

define void @Integer_Create_Default(ptr %0, i32 %1) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 %1, ptr %intFieldPtr, align 4
  ret void
}

define i32 @Integer_get(ptr %integerPtr) {
entry:
  %fieldPtr = getelementptr inbounds %Integer, ptr %integerPtr, i32 0, i32 0
  %value = load i32, ptr %fieldPtr, align 4
  ret i32 %value
}

define %Integer @Integer_Mult(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = mul i32 %selfValue, %otherValue
  store i32 %result, ptr %selfFieldPtr, align 4
  %returnValue = load %Integer, ptr %self, align 4
  ret %Integer %returnValue
}

define %Integer @Integer_Plus(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = add i32 %selfValue, %otherValue
  store i32 %result, ptr %selfFieldPtr, align 4
  %returnValue = load %Integer, ptr %self, align 4
  ret %Integer %returnValue
}

define %Integer @Integer_Minus(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = sub i32 %selfValue, %otherValue
  store i32 %result, ptr %selfFieldPtr, align 4
  %returnValue = load %Integer, ptr %self, align 4
  ret %Integer %returnValue
}

define %Integer @Integer_Div(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = sdiv i32 %selfValue, %otherValue
  store i32 %result, ptr %selfFieldPtr, align 4
  %returnValue = load %Integer, ptr %self, align 4
  ret %Integer %returnValue
}

define %Integer @Integer_Rem(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = srem i32 %selfValue, %otherValue
  store i32 %result, ptr %selfFieldPtr, align 4
  %returnValue = load %Integer, ptr %self, align 4
  ret %Integer %returnValue
}

define %Boolean @Integer_Less(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = icmp slt i32 %selfValue, %otherValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Create_Default(ptr %booleanResult, i1 %result)
  %loadedBoolean = load %Boolean, ptr %booleanResult, align 1
  ret %Boolean %loadedBoolean
}

define %Boolean @Integer_Greater(ptr %self, %Integer %other) {
entry:
  %selfFieldPtr = getelementptr inbounds %Integer, ptr %self, i32 0, i32 0
  %selfValue = load i32, ptr %selfFieldPtr, align 4
  %otherPtr = alloca %Integer, align 8
  store %Integer %other, ptr %otherPtr, align 4
  %otherFieldPtr = getelementptr inbounds %Integer, ptr %otherPtr, i32 0, i32 0
  %otherValue = load i32, ptr %otherFieldPtr, align 4
  %result = icmp sgt i32 %selfValue, %otherValue
  %booleanResult = alloca %Boolean, align 8
  call void @Boolean_Create_Default(ptr %booleanResult, i1 %result)
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

define void @Main_Create_Default(ptr %0) {
entry:
  %class_object = alloca %Integer, align 8
  call void @Integer_Create_Default(ptr %class_object)
  call void @Integer_scan(ptr %class_object)
  %class_object1 = alloca %Integer, align 8
  call void @Integer_Create_Default(ptr %class_object1)
  call void @Integer_scan(ptr %class_object1)
  %call_Greater = call %Boolean @Integer_Greater(ptr %class_object, i32 0)
  %alloca_return_val = alloca %Boolean, align 8
  store %Boolean %call_Greater, ptr %alloca_return_val, align 1
  %boolFieldPtr = getelementptr inbounds %Boolean, ptr %alloca_return_val, i32 0, i32 0
  %loadBoolValue = load i1, ptr %boolFieldPtr, align 1
  %whilecond = icmp ne i1 %loadBoolValue, false
  br i1 %whilecond, label %loop, label %afterloop

loop:                                             ; preds = %loop, %entry
  call void @Integer_print(ptr %class_object1)
  %call_Plus = call %Integer @Integer_Plus(ptr %class_object1, i32 1)
  %alloca_return_val2 = alloca %Integer, align 8
  store %Integer %call_Plus, ptr %alloca_return_val2, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %class_object1, ptr %alloca_return_val2, i64 8, i1 false)
  %call_Minus = call %Integer @Integer_Minus(ptr %class_object, i32 1)
  %alloca_return_val3 = alloca %Integer, align 8
  store %Integer %call_Minus, ptr %alloca_return_val3, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %class_object, ptr %alloca_return_val3, i64 8, i1 false)
  %call_Greater4 = call %Boolean @Integer_Greater(ptr %class_object, i32 0)
  %alloca_return_val5 = alloca %Boolean, align 8
  store %Boolean %call_Greater4, ptr %alloca_return_val5, align 1
  %boolFieldPtr6 = getelementptr inbounds %Boolean, ptr %alloca_return_val5, i32 0, i32 0
  %loadBoolValue7 = load i1, ptr %boolFieldPtr6, align 1
  %whilecond8 = icmp ne i1 %loadBoolValue7, false
  br i1 %whilecond8, label %loop, label %afterloop

afterloop:                                        ; preds = %loop, %entry
  call void @Integer_print(ptr %class_object1)
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #0

define i32 @main() {
entry:
  %main_object = alloca %Main, align 8
  call void @Main_Create_Default(ptr %main_object)
  ret i32 0
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
