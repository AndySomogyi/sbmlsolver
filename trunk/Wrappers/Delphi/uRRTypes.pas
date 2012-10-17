unit uRRTypes;

interface

type
  TDoubleArray = array of double;
  T2DDoubleArray = array of TDoubleArray;

  // Result tpe returned by simulate() and simulateEx()
  TRRResult = record
     RSize : integer;
     CSize : integer;
     Data : array of double;
     ColumnHeaders : ^PAnsiChar;
  end;
  PRRResultHandle = ^TRRResult;


  // Vector of double values
  TRRDoubleVector = record
       count : integer;
       data : array of double;
  end;
  PRRDoubleVectorHandle = ^TRRDoubleVector;


  // Matrix
  TRRMatrix = record
    RSize : integer;
    CSize : integer;
    data : array of double;
  end;
  PRRMatrixHandle = ^TRRMatrix;


  TRRCCodeAPI = record
    Header : PAnsiChar;
    Source : PAnsiChar;
  end;


  TRRCCode = record
    Header : AnsiString;
    Source : AnsiString;
  end;
  PRRCCodeHandle = ^TRRCCodeAPI;

  TListItemType = (litString, litInteger, litDouble, litList);

  PRRListItemRecord = ^TRRListItemRecord;
  PRRListRecordHandle = ^TRRListRecord;

  TRRListItemRecordArray = array[0..1000] of PRRListRecordHandle;
  PRRListItemRecordArray = ^TRRListItemRecordArray;

  TRRListItemRecord = record
      ItemType : TListItemType;
      case TListItemType of
         litInteger: (iValue : integer);
         litDouble:  (dValue : double);
         litString:  (sValue : PAnsiChar);
         litList:    (lValue : PRRListRecordHandle);
  end;

  TRRListRecord = record
       count : integer;
       Items : PRRListItemRecordArray;
  end;


implementation

end.
