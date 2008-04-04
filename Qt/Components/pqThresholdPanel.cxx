/*=========================================================================

   Program: ParaView
   Module:    pqThresholdPanel.cxx

   Copyright (c) 2005-2008 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.2. 

   See License_v1.2.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#include "pqThresholdPanel.h"

#include <QComboBox>
#include "pqDoubleRangeWidget.h"
#include "pqSMAdaptor.h"
#include "vtkSMProperty.h"
#include "vtkSMProxy.h"

pqThresholdPanel::pqThresholdPanel(pqProxy* pxy, QWidget* p) :
  pqLoadedFormObjectPanel(":/pqWidgets/UI/pqThresholdPanel.ui", pxy, p)
{
  this->Lower = this->findChild<pqDoubleRangeWidget*>("ThresholdBetween_0");
  this->Upper = this->findChild<pqDoubleRangeWidget*>("ThresholdBetween_1");

  QObject::connect(this->Lower, SIGNAL(valueEdited(double)),
                   this, SLOT(lowerChanged(double)));
  QObject::connect(this->Upper, SIGNAL(valueEdited(double)),
                   this, SLOT(upperChanged(double)));

  QObject::connect(this->findChild<QComboBox*>("SelectInputScalars"),
    SIGNAL(activated(int)), this, SLOT(variableChanged()),
    Qt::QueuedConnection);
}

pqThresholdPanel::~pqThresholdPanel()
{
}

void pqThresholdPanel::lowerChanged(double val)
{
  // clamp the lower threshold if we need to
  if(this->Upper->value() < val)
    {
    this->Upper->setValue(val);
    }
}

void pqThresholdPanel::upperChanged(double val)
{
  // clamp the lower threshold if we need to
  if(this->Lower->value() > val)
    {
    this->Lower->setValue(val);
    }
}

void pqThresholdPanel::variableChanged()
{
  // when the user changes the variable, adjust the ranges on the ThresholdBetween
  vtkSMProperty* prop = this->proxy()->GetProperty("ThresholdBetween");
  QList<QVariant> range = pqSMAdaptor::getElementPropertyDomain(prop);
  if(range.size() == 2 && range[0].isValid() && range[1].isValid())
    {
    this->Lower->setValue(range[0].toDouble());
    this->Upper->setValue(range[1].toDouble());
    }
}

