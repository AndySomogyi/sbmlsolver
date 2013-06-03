object capFrame: TcapFrame
  Left = 0
  Top = 0
  Width = 189
  Height = 163
  TabOrder = 0
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 189
    Height = 163
    Align = alClient
    TabOrder = 0
    object GroupBox3: TGroupBox
      Left = 1
      Top = 1
      Width = 187
      Height = 56
      Align = alTop
      Caption = 'Capability'
      Ctl3D = False
      ParentCtl3D = False
      TabOrder = 0
      object capDropDown: TComboBox
        Left = 3
        Top = 29
        Width = 171
        Height = 21
        AutoDropDown = True
        Style = csDropDownList
        TabOrder = 0
        OnChange = capDropDownChange
        OnClick = capDropDownClick
      end
    end
    object GroupBox4: TGroupBox
      Left = 1
      Top = 57
      Width = 187
      Height = 84
      Align = alTop
      Caption = 'Parameters'
      TabOrder = 1
      object parameterDropDown: TComboBox
        Left = 6
        Top = 24
        Width = 171
        Height = 21
        AutoDropDown = True
        Style = csDropDownList
        TabOrder = 0
        OnChange = parameterDropDownChange
      end
      object paraEdit: TEdit
        Left = 6
        Top = 53
        Width = 118
        Height = 21
        TabOrder = 1
        Text = 'paraEdit'
      end
      object SetParaBtn: TButton
        Left = 130
        Top = 51
        Width = 47
        Height = 25
        Caption = 'set'
        TabOrder = 2
        OnClick = SetParaBtnClick
      end
    end
  end
end
