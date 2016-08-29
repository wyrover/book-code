

class CSpinEdit {
        private:
                 float * value;
                 CSpinButtonCtrl * spin;
                 CEdit * edit;
                 virtual void GetValue();
                 virtual void SetValue();
                };

class CFunctionSpinEdit : public CSpinEdit {
                CComboBox * function;
                virtual void GetValue();
                virtual void SetValue();
                                           };
