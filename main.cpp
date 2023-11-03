void ArrayConstructorStubBuilder::GenerateCircuit() {
  GateRef glue = PtrArgument(static_cast<size_t>(BuiltinsArgs::GLUE));
  GateRef nativeCode =
      PtrArgument(static_cast<size_t>(BuiltinsArgs::NATIVECODE));
  GateRef func = TaggedArgument(static_cast<size_t>(BuiltinsArgs::FUNC));
  GateRef newTarget =
      TaggedArgument(static_cast<size_t>(BuiltinsArgs::NEWTARGET));
  GateRef thisValue =
      TaggedArgument(static_cast<size_t>(BuiltinsArgs::THISVALUE));
  GateRef numArgs = PtrArgument(static_cast<size_t>(BuiltinsArgs::NUMARGS));
  GenerateCircuitImpl(glue, nativeCode, func, newTarget, thisValue, numArgs);
}

void ArrayConstructorStubBuilder::GenerateCircuitImpl(GateRef glue,
                                                      GateRef nativeCode,
                                                      GateRef func,
                                                      GateRef newTarget,
                                                      GateRef thisValue,
                                                      GateRef numArgs) {
  auto env = GetEnvironment();
  DEFVARIABLE(res, VariableType::JS_ANY(), Undefined());

  Label newTargetIsHeapObject(env);
  Label newTargetIsJSFunction(env);
  Label slowPath(env);
  Label slowPath1(env);
  Label exit(env);

  Branch(TaggedIsHeapObject(newTarget), &newTargetIsHeapObject, &slowPath1);
  Bind(&newTargetIsHeapObject);
  Branch(IsJSFunction(newTarget), &newTargetIsJSFunction, &slowPath);
  Bind(&newTargetIsJSFunction);
  {
    Label fastGetHclass(env);
    Label intialHClassIsHClass(env);
    GateRef glueGlobalEnvOffset =
        IntPtr(JSThread::GlueData::GetGlueGlobalEnvOffset(env->Is32Bit()));
    GateRef glueGlobalEnv =
        Load(VariableType::NATIVE_POINTER(), glue, glueGlobalEnvOffset);
    auto arrayFunc = GetGlobalEnvValue(VariableType::JS_ANY(), glueGlobalEnv,
                                       GlobalEnv::ARRAY_FUNCTION_INDEX);
    Branch(Equal(arrayFunc, newTarget), &fastGetHclass, &slowPath);
    Bind(&fastGetHclass);
    GateRef intialHClass = Load(VariableType::JS_ANY(), newTarget,
                                IntPtr(JSFunction::PROTO_OR_DYNCLASS_OFFSET));
    DEFVARIABLE(arrayLength, VariableType::INT64(), Int64(0));
    Branch(IsJSHClass(intialHClass), &intialHClassIsHClass, &slowPath);
    Bind(&intialHClassIsHClass);
    {
      Label noArg(env);
      Label hasArg(env);
      Label arrayCreate(env);
      Branch(Int64Equal(numArgs, IntPtr(0)), &noArg, &hasArg);
      Bind(&noArg);
      { Jump(&arrayCreate); }
      Bind(&hasArg);
      {
        Label hasOneArg(env);
        Branch(Int64Equal(numArgs, IntPtr(1)), &hasOneArg, &slowPath);
        Bind(&hasOneArg);
        {
          Label argIsNumber(env);
          GateRef arg0 = GetArg(numArgs, IntPtr(0));
          Branch(TaggedIsNumber(arg0), &argIsNumber, &slowPath);
          Bind(&argIsNumber);
          {
            Label argIsInt(env);
            Label argIsDouble(env);
            Branch(TaggedIsInt(arg0), &argIsInt, &argIsDouble);
            Bind(&argIsInt);
            {
              Label validIntLength(env);
              GateRef intLen = GetInt64OfTInt(arg0);
              GateRef isGEZero = Int64GreaterThanOrEqual(intLen, Int64(0));
              GateRef isLEMaxLen =
                  Int64LessThanOrEqual(intLen, Int64(JSArray::MAX_ARRAY_INDEX));
              Branch(BoolAnd(isGEZero, isLEMaxLen), &validIntLength, &slowPath);
              Bind(&validIntLength);
              {
                arrayLength = intLen;
                Jump(&arrayCreate);
              }
            }
            Bind(&argIsDouble);
            {
              Label validDoubleLength(env);
              GateRef doubleLength = GetDoubleOfTDouble(arg0);
              GateRef doubleToInt = DoubleToInt(glue, doubleLength);
              GateRef intToDouble =
                  CastInt64ToFloat64(SExtInt32ToInt64(doubleToInt));
              GateRef doubleEqual = DoubleEqual(doubleLength, intToDouble);
              GateRef doubleLEMaxLen = DoubleLessThanOrEqual(
                  doubleLength, Double(JSArray::MAX_ARRAY_INDEX));
              Branch(BoolAnd(doubleEqual, doubleLEMaxLen), &validDoubleLength,
                     &slowPath);
              Bind(&validDoubleLength);
              {
                arrayLength = SExtInt32ToInt64(doubleToInt);
                Jump(&arrayCreate);
              }
            }
          }
        }
      }
      Bind(&arrayCreate);
      {
        Label lengthValid(env);
        Branch(Int64GreaterThan(*arrayLength, Int64(JSObject::MAX_GAP)),
               &slowPath, &lengthValid);
        Bind(&lengthValid);
        {
          NewObjectStubBuilder newBuilder(this);
          newBuilder.SetParameters(glue, 0);
          res = newBuilder.NewJSArrayWithSize(intialHClass, *arrayLength);
          GateRef lengthOffset = IntPtr(JSArray::LENGTH_OFFSET);
          Store(VariableType::INT32(), glue, *res, lengthOffset,
                TruncInt64ToInt32(*arrayLength));
          GateRef accessor =
              GetGlobalConstantValue(VariableType::JS_ANY(), glue,
                                     ConstantIndex::ARRAY_LENGTH_ACCESSOR);
          SetPropertyInlinedProps(glue, *res, intialHClass, accessor,
                                  Int32(JSArray::LENGTH_INLINE_PROPERTY_INDEX));
          SetExtensibleToBitfield(glue, *res, true);
          Jump(&exit);
        }
      }
    }
  }
  Bind(&slowPath);
  {
    auto name = BuiltinsStubCSigns::GetName(BUILTINS_STUB_ID(ArrayConstructor));
    GateRef argv = GetArgv();
    res = CallBuiltinRuntime(glue,
                             {glue, nativeCode, func, thisValue, numArgs, argv},
                             true, name.c_str());
    Jump(&exit);
  }
  Bind(&slowPath1);
  {
    auto name = BuiltinsStubCSigns::GetName(BUILTINS_STUB_ID(ArrayConstructor));
    res = CallSlowPath(nativeCode, glue, thisValue, numArgs, func, newTarget,
                       name.c_str());
    Jump(&exit);
  }

  Bind(&exit);
  Return(*res);
}