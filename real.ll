; ModuleID = 'Real'
source_filename = "real"

%Real = type { double }

@fmt = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

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
  %1 = call i32 (ptr, ...) @printf(ptr @fmt, double %loadDouble)
  ret void
}

declare i32 @printf(ptr, ...)