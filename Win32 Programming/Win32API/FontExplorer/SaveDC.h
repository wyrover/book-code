class CSaveDC : public CDC {
        public:
                CSaveDC(CDC * pdc) { dc = pdc; 
                                     m_hDC = m_hAttribDC = pdc->m_hDC; 
                                     saved = dc->SaveDC(); }
                ~CSaveDC() { dc->RestoreDC(saved); m_hDC = m_hAttribDC = NULL; }
                inline CDC * operator->(){ return dc; }
                inline operator CDC *() { return dc; }
                inline operator CDC &() { return *dc; }
        protected:
                CDC * dc;
                int saved;
                           };
