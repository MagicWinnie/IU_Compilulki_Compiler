; ModuleID = 'Integer'
source_filename = "integer"

%Integer = type { i32 }

@fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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
  %1 = call i32 (ptr, ...) @printf(ptr @fmt, i32 %loadInt)
  ret void
}




declare i32 @printf(ptr, ...)


