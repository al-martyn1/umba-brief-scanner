@set UINT8=--underlaying-type=std::uint8_t
@set UINT16=--underlaying-type=std::uint16_t
@set UINT32=--underlaying-type=std::uint32_t
@set INT=--underlaying-type=int

@set HEX2=--hex-width=2
@set HEX4=--hex-width=4
@set HEX6=--hex-width=6
@set HEX8=--hex-width=8

@rem set EXTRA=--enum-flags=extra
@rem set FLAGENUM_EXTRA=--enum-flags=serialize-set,deserialize-set

@set FLAGS=--enum-flags=flags
@set DECL=--enum-flags=type-decl
@set CLS=--enum-flags=enum-class
@set DECLCLS=--enum-flags=type-decl
@rem ,enum-class
@set FMTHEX=--enum-flags=fmt-hex
@set SERIALIZE=--enum-flags=serialize,deserialize,lowercase
@set SERIALIZE_FLAGS_ENUM=--enum-flags=serialize-set,deserialize-set


@set TPL_OVERRIDE=--override-template-parameter=EnumNameFormat:$(ENAMNAME)
@set GEN_OPTS=--enum-values-style=CamelStyle

@set VALUES_CAMEL=--enum-values-style=CamelStyle
@set SERIALIZE_CAMEL=--serialize-style=CamelStyle
@set VALUES_PASCAL=--enum-values-style=PascalStyle
@set SERIALIZE_PASCAL=--serialize-style=PascalStyle
@set SERIALIZE_HYPHEN=--serialize-style=HyphenStyle

@set GEN_FLAGS=--enum-flags=0 --enum-flags=type-decl, %VALUES_CAMEL% %SERIALIZE_PASCAL% --enum-serialize-style=All

@set ENUMERATION_DEFS=^
     -E=OutputFormat        -F=@OutputFormat.txt


umba-enum-gen %GEN_OPTS% %HEX2% %TPL_OVERRIDE% %GEN_FLAGS% ^
    %ENUMERATION_DEFS%                                                    ^
..\enums.h


@rem Что-то пошло не так и файл генерится в текущий каталог
if exist %~dp0enums.h copy /Y %~dp0enums.h %~dp0..\enums.h
if exist %~dp0enums.h del %~dp0enums.h

@rem if exist %~dp0enum_descriptions.h copy /Y %~dp0enum_descriptions.h %~dp0..\enum_descriptions.h
@rem if exist %~dp0enum_descriptions.h del %~dp0enum_descriptions.h
