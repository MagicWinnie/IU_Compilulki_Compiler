; ModuleID = 'compilul'ki'
source_filename = "compilul'ki"

%Boolean = type { i1 }
%Integer = type { i32 }
%Real = type { double }
%Main = type {}

@fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@fmt.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
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
  %extBool = zext i1 %loadBool to i32
  %1 = call i32 (ptr, ...) @printf(ptr @fmt, i32 %extBool)
  ret void
}

declare i32 @printf(ptr, ...)

define void @Integer_Create_Default(ptr %0, i32 %1) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  store i32 %1, ptr %intFieldPtr, align 4
  ret void
}

define void @Integer_print(ptr %0) {
entry:
  %intFieldPtr = getelementptr inbounds %Integer, ptr %0, i32 0, i32 0
  %loadInt = load i32, ptr %intFieldPtr, align 4
  %1 = call i32 (ptr, ...) @printf(ptr @fmt.1, i32 %loadInt)
  ret void
}

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
  %c = alloca %Real, align 8
  call void @Real_Create_Default(ptr %c, double 1.235000e+02)
  call void @Real_print(ptr %c)
  ret void
}

define i32 @main() {
entry:
  %main_object = alloca %Main, align 8
  call void @Main_Create_Default(ptr %main_object)
  ret i32 0
}
