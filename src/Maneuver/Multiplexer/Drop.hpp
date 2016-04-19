//***************************************************************************
// Copyright 2007-2015 Universidade do Porto - Faculdade de Engenharia      *
// Laboratório de Sistemas e Tecnologia Subaquática (LSTS)                  *
//***************************************************************************
// This file is part of DUNE: Unified Navigation Environment.               *
//                                                                          *
// Commercial Licence Usage                                                 *
// Licencees holding valid commercial DUNE licences may use this file in    *
// accordance with the commercial licence agreement provided with the       *
// Software or, alternatively, in accordance with the terms contained in a  *
// written agreement between you and Universidade do Porto. For licensing   *
// terms, conditions, and further information contact lsts@fe.up.pt.        *
//                                                                          *
// European Union Public Licence - EUPL v.1.1 Usage                         *
// Alternatively, this file may be used under the terms of the EUPL,        *
// Version 1.1 only (the "Licence"), appearing in the file LICENCE.md       *
// included in the packaging of this file. You may not use this work        *
// except in compliance with the Licence. Unless required by applicable     *
// law or agreed to in writing, software distributed under the Licence is   *
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF     *
// ANY KIND, either express or implied. See the Licence for the specific    *
// language governing permissions and limitations at                        *
// http://ec.europa.eu/idabc/eupl.html.                                     *
//***************************************************************************
// Author: Manuel Ribeiro                                                   *
//***************************************************************************

#ifndef MANEUVER_MULTIPLEXER_DROP_HPP_INCLUDED_
#define MANEUVER_MULTIPLEXER_DROP_HPP_INCLUDED_

#include <DUNE/DUNE.hpp>

// Local headers
#include "MuxedManeuver.hpp"

namespace Maneuver
{
  namespace Multiplexer
  {
    using DUNE_NAMESPACES;

    //! Drop maneuver
    class Drop: public MuxedManeuver<IMC::Drop, void>
    {
    public:
      //! Default constructor.
      //! @param[in] task pointer to Maneuver task
    	Drop(Maneuvers::Maneuver* task):
        MuxedManeuver<IMC::Drop, void>(task)
      { }

      //! Start maneuver function
      //! @param[in] maneuver drop maneuver message
      void
      onStart(const IMC::Drop* maneuver)
      {
        m_task->setControl(IMC::CL_PATH);

        IMC::DesiredPath path;
        path.end_lat = maneuver->lat;
        path.end_lon = maneuver->lon;
        path.end_z = maneuver->z;
        path.end_z_units = maneuver->z_units;
        path.speed = maneuver->speed;
        path.speed_units = maneuver->speed_units;

        m_task->dispatch(path);
      }

      //! On PathControlState message
      //! @param[in] pcs pointer to PathControlState message
      void
      onPathControlState(const IMC::PathControlState* pcs)
      {
        if (pcs->flags & IMC::PathControlState::FL_NEAR)
        {
          IMC::SetServoPosition setServo;
          setServo.id = 0;
          setServo.value = 0;
          m_task->dispatch(setServo);
          m_task->signalCompletion();
        }
        else
          m_task->signalProgress(pcs->eta);
      }

      ~Drop(void)
      { }
    };
  }
}

#endif
