/*
 * helicopter.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "helicopter".
 *
 * Model version              : 11.5
 * Simulink Coder version : 9.4 (R2020b) 29-Jul-2020
 * C source code generated on : Wed Feb 11 15:51:11 2026
 *
 * Target selection: quarc_win64.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: 32-bit Generic
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "helicopter.h"
#include "helicopter_private.h"
#include "helicopter_dt.h"

/* Block signals (default storage) */
B_helicopter_T helicopter_B;

/* Continuous states */
X_helicopter_T helicopter_X;

/* Block states (default storage) */
DW_helicopter_T helicopter_DW;

/* Real-time model */
static RT_MODEL_helicopter_T helicopter_M_;
RT_MODEL_helicopter_T *const helicopter_M = &helicopter_M_;

/*
 * Writes out MAT-file header.  Returns success or failure.
 * Returns:
 *      0 - success
 *      1 - failure
 */
int_T rt_WriteMat4FileHeader(FILE *fp, int32_T m, int32_T n, const char *name)
{
  typedef enum { ELITTLE_ENDIAN, EBIG_ENDIAN } ByteOrder;

  int16_T one = 1;
  ByteOrder byteOrder = (*((int8_T *)&one)==1) ? ELITTLE_ENDIAN : EBIG_ENDIAN;
  int32_T type = (byteOrder == ELITTLE_ENDIAN) ? 0: 1000;
  int32_T imagf = 0;
  int32_T name_len = (int32_T)strlen(name) + 1;
  if ((fwrite(&type, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&m, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&n, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&imagf, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(&name_len, sizeof(int32_T), 1, fp) == 0) ||
      (fwrite(name, sizeof(char), name_len, fp) == 0)) {
    return(1);
  } else {
    return(0);
  }
}                                      /* end rt_WriteMat4FileHeader */

/*
 * This function updates continuous states using the ODE1 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE1_IntgData *id = (ODE1_IntgData *)rtsiGetSolverData(si);
  real_T *f0 = id->f[0];
  int_T i;
  int_T nXc = 4;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);
  rtsiSetdX(si, f0);
  helicopter_derivatives();
  rtsiSetT(si, tnew);
  for (i = 0; i < nXc; ++i) {
    x[i] += h * f0[i];
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model output function */
void helicopter_output(void)
{
  /* local block i/o variables */
  real_T rtb_FromFile2;
  real_T rtb_Frontgain;
  real_T rtb_Derivative;
  real_T rtb_Backgain;
  real_T rtb_HILReadEncoderTimebase_o1;
  real_T rtb_HILReadEncoderTimebase_o2;
  real_T rtb_HILReadEncoderTimebase_o3;
  real_T rtb_TmpSignalConversionAtToFile[6];
  real_T rtb_Gain1_idx_2;
  real_T rtb_Gain1_idx_3;
  real_T rtb_Sum;
  real_T *lastU;
  int8_T rtAction;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* set solver stop time */
    if (!(helicopter_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTickH0 + 1) *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&helicopter_M->solverInfo,
                            ((helicopter_M->Timing.clockTick0 + 1) *
        helicopter_M->Timing.stepSize0 + helicopter_M->Timing.clockTickH0 *
        helicopter_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(helicopter_M)) {
    helicopter_M->Timing.t[0] = rtsiGetT(&helicopter_M->solverInfo);
  }

  /* Reset subsysRan breadcrumbs */
  srClearBC(helicopter_DW.IfActionSubsystem_SubsysRanBC);
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
    {
      t_error result;
      result = hil_task_read_encoder(helicopter_DW.HILReadEncoderTimebase_Task,
        1, &helicopter_DW.HILReadEncoderTimebase_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      } else {
        rtb_HILReadEncoderTimebase_o1 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[0];
        rtb_HILReadEncoderTimebase_o2 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[1];
        rtb_HILReadEncoderTimebase_o3 =
          helicopter_DW.HILReadEncoderTimebase_Buffer[2];
      }
    }
  }

  /* Integrator: '<S3>/Integrator' */
  /* Limited  Integrator  */
  if (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat) {
    helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_UpperSat;
  } else {
    if (helicopter_X.Integrator_CSTATE <= helicopter_P.Integrator_LowerSat) {
      helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_LowerSat;
    }
  }

  /* FromFile: '<Root>/From File2' incorporates:
   *  Integrator: '<S3>/Integrator'
   */
  rtb_FromFile2 = helicopter_X.Integrator_CSTATE;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* Gain: '<S4>/Travel: Count to rad' incorporates:
     *  Gain: '<S4>/Travel_gain'
     */
    helicopter_B.TravelCounttorad = helicopter_P.travel_gain *
      rtb_HILReadEncoderTimebase_o1 * helicopter_P.TravelCounttorad_Gain;

    /* Gain: '<S12>/Gain' */
    helicopter_B.Gain = helicopter_P.Gain_Gain * helicopter_B.TravelCounttorad;

    /* Gain: '<S4>/Pitch: Count to rad' */
    helicopter_B.PitchCounttorad = helicopter_P.PitchCounttorad_Gain *
      rtb_HILReadEncoderTimebase_o2;

    /* Gain: '<S9>/Gain' */
    helicopter_B.Gain_i = helicopter_P.Gain_Gain_a *
      helicopter_B.PitchCounttorad;
  }

  /* Gain: '<S13>/Gain' incorporates:
   *  TransferFcn: '<S4>/Travel: Transfer Fcn'
   */
  helicopter_B.Gain_d = (helicopter_P.TravelTransferFcn_C *
    helicopter_X.TravelTransferFcn_CSTATE + helicopter_P.TravelTransferFcn_D *
    helicopter_B.TravelCounttorad) * helicopter_P.Gain_Gain_l;

  /* Gain: '<S10>/Gain' incorporates:
   *  TransferFcn: '<S4>/Pitch: Transfer Fcn'
   */
  helicopter_B.Gain_b = (helicopter_P.PitchTransferFcn_C *
    helicopter_X.PitchTransferFcn_CSTATE + helicopter_P.PitchTransferFcn_D *
    helicopter_B.PitchCounttorad) * helicopter_P.Gain_Gain_ae;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* Gain: '<S4>/Elevation: Count to rad' incorporates:
     *  Gain: '<S4>/Elevation_gain'
     */
    helicopter_B.ElevationCounttorad = helicopter_P.elevation_gain *
      rtb_HILReadEncoderTimebase_o3 * helicopter_P.ElevationCounttorad_Gain;

    /* Gain: '<S7>/Gain' */
    helicopter_B.Gain_e = helicopter_P.Gain_Gain_lv *
      helicopter_B.ElevationCounttorad;

    /* Sum: '<Root>/Sum' incorporates:
     *  Constant: '<Root>/elavation_offset [deg]'
     */
    helicopter_B.Sum = helicopter_B.Gain_e +
      helicopter_P.elavation_offsetdeg_Value;
  }

  /* Gain: '<S8>/Gain' incorporates:
   *  TransferFcn: '<S4>/Elevation: Transfer Fcn'
   */
  helicopter_B.Gain_dg = (helicopter_P.ElevationTransferFcn_C *
    helicopter_X.ElevationTransferFcn_CSTATE +
    helicopter_P.ElevationTransferFcn_D * helicopter_B.ElevationCounttorad) *
    helicopter_P.Gain_Gain_n;

  /* Gain: '<S2>/Gain1' */
  rtb_Gain1_idx_2 = helicopter_P.Gain1_Gain * helicopter_B.Gain_i;
  rtb_Gain1_idx_3 = helicopter_P.Gain1_Gain * helicopter_B.Gain_b;

  /* Sum: '<S3>/Sum' incorporates:
   *  Constant: '<Root>/elevation_ref'
   *  Gain: '<S2>/Gain1'
   */
  rtb_Sum = helicopter_P.elevation_ref_Value - helicopter_P.Gain1_Gain *
    helicopter_B.Sum;

  /* Derivative: '<S4>/Derivative' incorporates:
   *  Gain: '<S2>/Gain1'
   *  Gain: '<S3>/K_ed'
   */
  rtb_Derivative = helicopter_P.Gain1_Gain * helicopter_B.Gain_dg *
    helicopter_P.K_ed;

  /* Sum: '<Root>/Sum2' incorporates:
   *  Constant: '<Root>/Vs_bias'
   *  Gain: '<S3>/K_ep'
   *  Sum: '<S3>/Sum1'
   */
  helicopter_B.Sum2 = ((helicopter_P.K_ep * rtb_Sum + rtb_FromFile2) -
                       rtb_Derivative) + helicopter_P.Vs_ff;

  /* FromFile: '<Root>/From File1' */
  {
    static const real_T *pStart = (NULL);
    static boolean_T initBasePtr = true;
    real_T time = helicopter_M->Timing.t[0];
    const real_T *pU = (NULL);
    const real_T *pT = (const real_T *)
      helicopter_DW.FromFile1_PWORK.PrevTimePtr;
    if (initBasePtr == true) {
      pStart = (real_T *) helicopter_DW.FromFile1_PWORK.PrevTimePtr;
      initBasePtr = false;
    }

    pU = pStart + 179;
    if (time <= pStart[0]) {
      pT = pStart;
    } else if (time >= pU[0]) {
      pT = pU;
    } else {
      if (time < pT[0]) {
        while (time < pT[0]) {
          pT--;
        }
      } else {
        while (time >= pT[1]) {
          pT++;
        }
      }
    }

    helicopter_DW.FromFile1_PWORK.PrevTimePtr = (void *) pT;
    pU = pT + 181;
    if (pT[0] == pT[1]) {
      rtb_Derivative = pU[ (time < pT[0]) ? 0 : 1 ];
    } else {
      real_T f = (pT[1]-time)/(pT[1]-pT[0]);
      if (pU[0] == pU[1]) {
        rtb_Derivative = pU[0];
      } else {
        rtb_Derivative = f*pU[0]+(1.0-f)*pU[1];
      }
    }
  }

  /* Derivative: '<S4>/Derivative' incorporates:
   *  Constant: '<Root>/Constant1'
   *  Sum: '<Root>/Sum5'
   */
  rtb_Derivative -= helicopter_P.Constant1_Value;

  /* FromFile: '<Root>/From File2' */
  {
    static const real_T *pStart = (NULL);
    static boolean_T initBasePtr = true;
    real_T time = helicopter_M->Timing.t[0];
    const real_T *pU = (NULL);
    const real_T *pT = (const real_T *)
      helicopter_DW.FromFile2_PWORK.PrevTimePtr;
    if (initBasePtr == true) {
      pStart = (real_T *) helicopter_DW.FromFile2_PWORK.PrevTimePtr;
      initBasePtr = false;
    }

    pU = pStart + 179;
    if (time <= pStart[0]) {
      pT = pStart;
    } else if (time >= pU[0]) {
      pT = pU;
    } else {
      if (time < pT[0]) {
        while (time < pT[0]) {
          pT--;
        }
      } else {
        while (time >= pT[1]) {
          pT++;
        }
      }
    }

    helicopter_DW.FromFile2_PWORK.PrevTimePtr = (void *) pT;
    pU = pT + 181;
    if (pT[0] == pT[1]) {
      rtb_FromFile2 = pU[ (time < pT[0]) ? 0 : 1 ];
    } else {
      real_T f = (pT[1]-time)/(pT[1]-pT[0]);
      if (pU[0] == pU[1]) {
        rtb_FromFile2 = pU[0];
      } else {
        rtb_FromFile2 = f*pU[0]+(1.0-f)*pU[1];
      }
    }
  }

  /* FromFile: '<Root>/From File3' */
  {
    static const real_T *pStart = (NULL);
    static boolean_T initBasePtr = true;
    real_T time = helicopter_M->Timing.t[0];
    const real_T *pU = (NULL);
    const real_T *pT = (const real_T *)
      helicopter_DW.FromFile3_PWORK.PrevTimePtr;
    if (initBasePtr == true) {
      pStart = (real_T *) helicopter_DW.FromFile3_PWORK.PrevTimePtr;
      initBasePtr = false;
    }

    pU = pStart + 179;
    if (time <= pStart[0]) {
      pT = pStart;
    } else if (time >= pU[0]) {
      pT = pU;
    } else {
      if (time < pT[0]) {
        while (time < pT[0]) {
          pT--;
        }
      } else {
        while (time >= pT[1]) {
          pT++;
        }
      }
    }

    helicopter_DW.FromFile3_PWORK.PrevTimePtr = (void *) pT;
    pU = pT + 181;
    if (pT[0] == pT[1]) {
      rtb_Frontgain = pU[ (time < pT[0]) ? 0 : 1 ];
    } else {
      real_T f = (pT[1]-time)/(pT[1]-pT[0]);
      if (pU[0] == pU[1]) {
        rtb_Frontgain = pU[0];
      } else {
        rtb_Frontgain = f*pU[0]+(1.0-f)*pU[1];
      }
    }
  }

  /* FromFile: '<Root>/From File4' */
  {
    static const real_T *pStart = (NULL);
    static boolean_T initBasePtr = true;
    real_T time = helicopter_M->Timing.t[0];
    const real_T *pU = (NULL);
    const real_T *pT = (const real_T *)
      helicopter_DW.FromFile4_PWORK.PrevTimePtr;
    if (initBasePtr == true) {
      pStart = (real_T *) helicopter_DW.FromFile4_PWORK.PrevTimePtr;
      initBasePtr = false;
    }

    pU = pStart + 179;
    if (time <= pStart[0]) {
      pT = pStart;
    } else if (time >= pU[0]) {
      pT = pU;
    } else {
      if (time < pT[0]) {
        while (time < pT[0]) {
          pT--;
        }
      } else {
        while (time >= pT[1]) {
          pT++;
        }
      }
    }

    helicopter_DW.FromFile4_PWORK.PrevTimePtr = (void *) pT;
    pU = pT + 181;
    if (pT[0] == pT[1]) {
      rtb_Backgain = pU[ (time < pT[0]) ? 0 : 1 ];
    } else {
      real_T f = (pT[1]-time)/(pT[1]-pT[0]);
      if (pU[0] == pU[1]) {
        rtb_Backgain = pU[0];
      } else {
        rtb_Backgain = f*pU[0]+(1.0-f)*pU[1];
      }
    }
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Gain: '<Root>/Gain'
   *  Gain: '<S2>/Gain1'
   *  Sum: '<Root>/Sum3'
   */
  rtb_Backgain = (((helicopter_P.Gain1_Gain * helicopter_B.Gain - rtb_Derivative)
                   * helicopter_P.K_opt[0] + (helicopter_P.Gain1_Gain *
    helicopter_B.Gain_d - rtb_FromFile2) * helicopter_P.K_opt[1]) +
                  (rtb_Gain1_idx_2 - rtb_Frontgain) * helicopter_P.K_opt[2]) +
    (rtb_Gain1_idx_3 - rtb_Backgain) * helicopter_P.K_opt[3];

  /* FromFile: '<Root>/From File5' */
  {
    static const real_T *pStart = (NULL);
    static boolean_T initBasePtr = true;
    real_T time = helicopter_M->Timing.t[0];
    const real_T *pU = (NULL);
    const real_T *pT = (const real_T *)
      helicopter_DW.FromFile5_PWORK.PrevTimePtr;
    if (initBasePtr == true) {
      pStart = (real_T *) helicopter_DW.FromFile5_PWORK.PrevTimePtr;
      initBasePtr = false;
    }

    pU = pStart + 179;
    if (time <= pStart[0]) {
      pT = pStart;
    } else if (time >= pU[0]) {
      pT = pU;
    } else {
      if (time < pT[0]) {
        while (time < pT[0]) {
          pT--;
        }
      } else {
        while (time >= pT[1]) {
          pT++;
        }
      }
    }

    helicopter_DW.FromFile5_PWORK.PrevTimePtr = (void *) pT;
    pU = pT + 181;
    if (pT[0] == pT[1]) {
      rtb_Frontgain = pU[ (time < pT[0]) ? 0 : 1 ];
    } else {
      real_T f = (pT[1]-time)/(pT[1]-pT[0]);
      if (pU[0] == pU[1]) {
        rtb_Frontgain = pU[0];
      } else {
        rtb_Frontgain = f*pU[0]+(1.0-f)*pU[1];
      }
    }
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<Root>/Sum4'
   *  Sum: '<S5>/Sum2'
   */
  rtb_Backgain = rtb_Frontgain - rtb_Backgain;
  rtb_Backgain -= rtb_Gain1_idx_2;

  /* Gain: '<S5>/K_pp' */
  rtb_Gain1_idx_2 = helicopter_P.K_pp * rtb_Backgain;

  /* Gain: '<S1>/Back gain' incorporates:
   *  Gain: '<S5>/K_pd'
   */
  rtb_Backgain = helicopter_P.K_pd * rtb_Gain1_idx_3;

  /* Sum: '<Root>/Sum1' incorporates:
   *  Constant: '<Root>/Constant'
   *  Constant: '<Root>/Vd_bias'
   *  Sum: '<S5>/Sum3'
   */
  helicopter_B.Sum1 = ((rtb_Gain1_idx_2 - rtb_Backgain) + helicopter_P.Vd_ff) +
    helicopter_P.Constant_Value;
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* SignalConversion generated from: '<Root>/To File' */
    rtb_TmpSignalConversionAtToFile[0] = helicopter_B.Gain;
    rtb_TmpSignalConversionAtToFile[1] = helicopter_B.Gain_d;
    rtb_TmpSignalConversionAtToFile[2] = helicopter_B.Gain_i;
    rtb_TmpSignalConversionAtToFile[3] = helicopter_B.Gain_b;
    rtb_TmpSignalConversionAtToFile[4] = helicopter_B.Sum;
    rtb_TmpSignalConversionAtToFile[5] = helicopter_B.Gain_dg;

    /* ToFile: '<Root>/To File' */
    {
      if (!(++helicopter_DW.ToFile_IWORK.Decimation % 1) &&
          (helicopter_DW.ToFile_IWORK.Count * (6 + 1)) + 1 < 100000000 ) {
        FILE *fp = (FILE *) helicopter_DW.ToFile_PWORK.FilePtr;
        if (fp != (NULL)) {
          real_T u[6 + 1];
          helicopter_DW.ToFile_IWORK.Decimation = 0;
          u[0] = helicopter_M->Timing.t[1];
          u[1] = rtb_TmpSignalConversionAtToFile[0];
          u[2] = rtb_TmpSignalConversionAtToFile[1];
          u[3] = rtb_TmpSignalConversionAtToFile[2];
          u[4] = rtb_TmpSignalConversionAtToFile[3];
          u[5] = rtb_TmpSignalConversionAtToFile[4];
          u[6] = rtb_TmpSignalConversionAtToFile[5];
          if (fwrite(u, sizeof(real_T), 6 + 1, fp) != 6 + 1) {
            rtmSetErrorStatus(helicopter_M,
                              "Error writing to MAT-file dag3_q_001_r_1.mat");
            return;
          }

          if (((++helicopter_DW.ToFile_IWORK.Count) * (6 + 1))+1 >= 100000000) {
            (void)fprintf(stdout,
                          "*** The ToFile block will stop logging data before\n"
                          "    the simulation has ended, because it has reached\n"
                          "    the maximum number of elements (100000000)\n"
                          "    allowed in MAT-file dag3_q_001_r_1.mat.\n");
          }
        }
      }
    }
  }

  /* Gain: '<S1>/Back gain' incorporates:
   *  Sum: '<S1>/Subtract'
   */
  rtb_Backgain = (helicopter_B.Sum2 - helicopter_B.Sum1) *
    helicopter_P.Backgain_Gain;

  /* Gain: '<S1>/Front gain' incorporates:
   *  Sum: '<S1>/Add'
   */
  rtb_Frontgain = (helicopter_B.Sum1 + helicopter_B.Sum2) *
    helicopter_P.Frontgain_Gain;

  /* Clock: '<S3>/Clock' incorporates:
   *  Derivative: '<S4>/Derivative'
   */
  rtb_Gain1_idx_3 = helicopter_M->Timing.t[0];

  /* Derivative: '<S4>/Derivative' incorporates:
   *  Clock: '<S3>/Clock'
   */
  rtb_Derivative = rtb_Gain1_idx_3;

  /* If: '<S3>/If' incorporates:
   *  Gain: '<S3>/K_ei'
   *  Inport: '<S6>/In1'
   */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    rtAction = (int8_T)!(rtb_Derivative >= 2.0);
    helicopter_DW.If_ActiveSubsystem = rtAction;
  } else {
    rtAction = helicopter_DW.If_ActiveSubsystem;
  }

  if (rtAction == 0) {
    /* Outputs for IfAction SubSystem: '<S3>/If Action Subsystem' incorporates:
     *  ActionPort: '<S6>/Action Port'
     */
    helicopter_B.In1 = helicopter_P.K_ei * rtb_Sum;
    if (rtmIsMajorTimeStep(helicopter_M)) {
      srUpdateBC(helicopter_DW.IfActionSubsystem_SubsysRanBC);
    }

    /* End of Outputs for SubSystem: '<S3>/If Action Subsystem' */
  }

  /* End of If: '<S3>/If' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Derivative: '<S4>/Derivative' */
  if ((helicopter_DW.TimeStampA >= rtb_Gain1_idx_3) && (helicopter_DW.TimeStampB
       >= rtb_Gain1_idx_3)) {
    /* Derivative: '<S4>/Derivative' */
    rtb_Derivative = 0.0;
  } else {
    rtb_Sum = helicopter_DW.TimeStampA;
    lastU = &helicopter_DW.LastUAtTimeA;
    if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
      if (helicopter_DW.TimeStampB < rtb_Gain1_idx_3) {
        rtb_Sum = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    } else {
      if (helicopter_DW.TimeStampA >= rtb_Gain1_idx_3) {
        rtb_Sum = helicopter_DW.TimeStampB;
        lastU = &helicopter_DW.LastUAtTimeB;
      }
    }

    /* Derivative: '<S4>/Derivative' */
    rtb_Derivative = (helicopter_B.PitchCounttorad - *lastU) / (rtb_Gain1_idx_3
      - rtb_Sum);
  }

  /* Gain: '<S11>/Gain' */
  helicopter_B.Gain_l = helicopter_P.Gain_Gain_a1 * rtb_Derivative;
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Saturate: '<S4>/Back motor: Saturation' */
  if (rtb_Backgain > helicopter_P.BackmotorSaturation_UpperSat) {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_UpperSat;
  } else if (rtb_Backgain < helicopter_P.BackmotorSaturation_LowerSat) {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = helicopter_P.BackmotorSaturation_LowerSat;
  } else {
    /* Saturate: '<S4>/Back motor: Saturation' */
    helicopter_B.BackmotorSaturation = rtb_Backgain;
  }

  /* End of Saturate: '<S4>/Back motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
  }

  /* Saturate: '<S4>/Front motor: Saturation' */
  if (rtb_Frontgain > helicopter_P.FrontmotorSaturation_UpperSat) {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_UpperSat;
  } else if (rtb_Frontgain < helicopter_P.FrontmotorSaturation_LowerSat) {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation =
      helicopter_P.FrontmotorSaturation_LowerSat;
  } else {
    /* Saturate: '<S4>/Front motor: Saturation' */
    helicopter_B.FrontmotorSaturation = rtb_Frontgain;
  }

  /* End of Saturate: '<S4>/Front motor: Saturation' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    /* S-Function (hil_write_analog_block): '<S4>/HIL Write Analog' */

    /* S-Function Block: helicopter/Helicopter_interface/HIL Write Analog (hil_write_analog_block) */
    {
      t_error result;
      helicopter_DW.HILWriteAnalog_Buffer[0] = helicopter_B.FrontmotorSaturation;
      helicopter_DW.HILWriteAnalog_Buffer[1] = helicopter_B.BackmotorSaturation;
      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILWriteAnalog_channels, 2,
        &helicopter_DW.HILWriteAnalog_Buffer[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
      }
    }
  }
}

/* Model update function */
void helicopter_update(void)
{
  real_T *lastU;

  /* Update for Derivative: '<S4>/Derivative' */
  if (helicopter_DW.TimeStampA == (rtInf)) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else if (helicopter_DW.TimeStampB == (rtInf)) {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  } else if (helicopter_DW.TimeStampA < helicopter_DW.TimeStampB) {
    helicopter_DW.TimeStampA = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeA;
  } else {
    helicopter_DW.TimeStampB = helicopter_M->Timing.t[0];
    lastU = &helicopter_DW.LastUAtTimeB;
  }

  *lastU = helicopter_B.PitchCounttorad;

  /* End of Update for Derivative: '<S4>/Derivative' */
  if (rtmIsMajorTimeStep(helicopter_M)) {
    rt_ertODEUpdateContinuousStates(&helicopter_M->solverInfo);
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++helicopter_M->Timing.clockTick0)) {
    ++helicopter_M->Timing.clockTickH0;
  }

  helicopter_M->Timing.t[0] = rtsiGetSolverStopTime(&helicopter_M->solverInfo);

  {
    /* Update absolute timer for sample time: [0.002s, 0.0s] */
    /* The "clockTick1" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick1"
     * and "Timing.stepSize1". Size of "clockTick1" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick1 and the high bits
     * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++helicopter_M->Timing.clockTick1)) {
      ++helicopter_M->Timing.clockTickH1;
    }

    helicopter_M->Timing.t[1] = helicopter_M->Timing.clockTick1 *
      helicopter_M->Timing.stepSize1 + helicopter_M->Timing.clockTickH1 *
      helicopter_M->Timing.stepSize1 * 4294967296.0;
  }
}

/* Derivatives for root system: '<Root>' */
void helicopter_derivatives(void)
{
  XDot_helicopter_T *_rtXdot;
  boolean_T lsat;
  boolean_T usat;
  _rtXdot = ((XDot_helicopter_T *) helicopter_M->derivs);

  /* Derivatives for Integrator: '<S3>/Integrator' */
  lsat = (helicopter_X.Integrator_CSTATE <= helicopter_P.Integrator_LowerSat);
  usat = (helicopter_X.Integrator_CSTATE >= helicopter_P.Integrator_UpperSat);
  if (((!lsat) && (!usat)) || (lsat && (helicopter_B.In1 > 0.0)) || (usat &&
       (helicopter_B.In1 < 0.0))) {
    _rtXdot->Integrator_CSTATE = helicopter_B.In1;
  } else {
    /* in saturation */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S3>/Integrator' */

  /* Derivatives for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  _rtXdot->TravelTransferFcn_CSTATE = 0.0;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_P.TravelTransferFcn_A *
    helicopter_X.TravelTransferFcn_CSTATE;
  _rtXdot->TravelTransferFcn_CSTATE += helicopter_B.TravelCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  _rtXdot->PitchTransferFcn_CSTATE = 0.0;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_P.PitchTransferFcn_A *
    helicopter_X.PitchTransferFcn_CSTATE;
  _rtXdot->PitchTransferFcn_CSTATE += helicopter_B.PitchCounttorad;

  /* Derivatives for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  _rtXdot->ElevationTransferFcn_CSTATE = 0.0;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_P.ElevationTransferFcn_A *
    helicopter_X.ElevationTransferFcn_CSTATE;
  _rtXdot->ElevationTransferFcn_CSTATE += helicopter_B.ElevationCounttorad;
}

/* Model initialize function */
void helicopter_initialize(void)
{
  /* Start for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_int result;
    t_boolean is_switching;
    result = hil_open("q8_usb", "0", &helicopter_DW.HILInitialize_Card);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    is_switching = false;
    result = hil_set_card_specific_options(helicopter_DW.HILInitialize_Card,
      "update_rate=normal;decimation=1", 32);
    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    result = hil_watchdog_clear(helicopter_DW.HILInitialize_Card);
    if (result < 0 && result != -QERR_HIL_WATCHDOG_CLEAR) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
      return;
    }

    if ((helicopter_P.HILInitialize_AIPStart && !is_switching) ||
        (helicopter_P.HILInitialize_AIPEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AIMinimums = &helicopter_DW.HILInitialize_AIMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMinimums[i1] = (helicopter_P.HILInitialize_AILow);
        }
      }

      {
        int_T i1;
        real_T *dw_AIMaximums = &helicopter_DW.HILInitialize_AIMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AIMaximums[i1] = helicopter_P.HILInitialize_AIHigh;
        }
      }

      result = hil_set_analog_input_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AIChannels, 8U,
        &helicopter_DW.HILInitialize_AIMinimums[0],
        &helicopter_DW.HILInitialize_AIMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_AOPStart && !is_switching) ||
        (helicopter_P.HILInitialize_AOPEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOMinimums = &helicopter_DW.HILInitialize_AOMinimums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMinimums[i1] = (helicopter_P.HILInitialize_AOLow);
        }
      }

      {
        int_T i1;
        real_T *dw_AOMaximums = &helicopter_DW.HILInitialize_AOMaximums[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOMaximums[i1] = helicopter_P.HILInitialize_AOHigh;
        }
      }

      result = hil_set_analog_output_ranges(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AOChannels, 8U,
        &helicopter_DW.HILInitialize_AOMinimums[0],
        &helicopter_DW.HILInitialize_AOMaximums[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_AOStart && !is_switching) ||
        (helicopter_P.HILInitialize_AOEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOInitial;
        }
      }

      result = hil_write_analog(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_AOChannels, 8U,
        &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_AOReset) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOWatchdog;
        }
      }

      result = hil_watchdog_set_analog_expiration_state
        (helicopter_DW.HILInitialize_Card, helicopter_P.HILInitialize_AOChannels,
         8U, &helicopter_DW.HILInitialize_AOVoltages[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_EIPStart && !is_switching) ||
        (helicopter_P.HILInitialize_EIPEnter && is_switching)) {
      {
        int_T i1;
        int32_T *dw_QuadratureModes =
          &helicopter_DW.HILInitialize_QuadratureModes[0];
        for (i1=0; i1 < 8; i1++) {
          dw_QuadratureModes[i1] = helicopter_P.HILInitialize_EIQuadrature;
        }
      }

      result = hil_set_encoder_quadrature_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_EIChannels, 8U, (t_encoder_quadrature_mode *)
        &helicopter_DW.HILInitialize_QuadratureModes[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_EIStart && !is_switching) ||
        (helicopter_P.HILInitialize_EIEnter && is_switching)) {
      {
        int_T i1;
        int32_T *dw_InitialEICounts =
          &helicopter_DW.HILInitialize_InitialEICounts[0];
        for (i1=0; i1 < 8; i1++) {
          dw_InitialEICounts[i1] = helicopter_P.HILInitialize_EIInitial;
        }
      }

      result = hil_set_encoder_counts(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_EIChannels, 8U,
        &helicopter_DW.HILInitialize_InitialEICounts[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_POPStart && !is_switching) ||
        (helicopter_P.HILInitialize_POPEnter && is_switching)) {
      uint32_T num_duty_cycle_modes = 0;
      uint32_T num_frequency_modes = 0;

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_POModes;
        }
      }

      result = hil_set_pwm_mode(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U, (t_pwm_mode *)
        &helicopter_DW.HILInitialize_POModeValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        const uint32_T *p_HILInitialize_POChannels =
          helicopter_P.HILInitialize_POChannels;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          if (dw_POModeValues[i1] == PWM_DUTY_CYCLE_MODE || dw_POModeValues[i1] ==
              PWM_ONE_SHOT_MODE || dw_POModeValues[i1] == PWM_TIME_MODE ||
              dw_POModeValues[i1] == PWM_RAW_MODE) {
            helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes] =
              (p_HILInitialize_POChannels[i1]);
            helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes] =
              helicopter_P.HILInitialize_POFrequency;
            num_duty_cycle_modes++;
          } else {
            helicopter_DW.HILInitialize_POSortedChans[7U - num_frequency_modes] =
              (p_HILInitialize_POChannels[i1]);
            helicopter_DW.HILInitialize_POSortedFreqs[7U - num_frequency_modes] =
              helicopter_P.HILInitialize_POFrequency;
            num_frequency_modes++;
          }
        }
      }

      if (num_duty_cycle_modes > 0) {
        result = hil_set_pwm_frequency(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[0], num_duty_cycle_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[0]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      if (num_frequency_modes > 0) {
        result = hil_set_pwm_duty_cycle(helicopter_DW.HILInitialize_Card,
          &helicopter_DW.HILInitialize_POSortedChans[num_duty_cycle_modes],
          num_frequency_modes,
          &helicopter_DW.HILInitialize_POSortedFreqs[num_duty_cycle_modes]);
        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
          return;
        }
      }

      {
        int_T i1;
        int32_T *dw_POModeValues = &helicopter_DW.HILInitialize_POModeValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POModeValues[i1] = helicopter_P.HILInitialize_POConfiguration;
        }
      }

      {
        int_T i1;
        int32_T *dw_POAlignValues = &helicopter_DW.HILInitialize_POAlignValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POAlignValues[i1] = helicopter_P.HILInitialize_POAlignment;
        }
      }

      {
        int_T i1;
        int32_T *dw_POPolarityVals =
          &helicopter_DW.HILInitialize_POPolarityVals[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POPolarityVals[i1] = helicopter_P.HILInitialize_POPolarity;
        }
      }

      result = hil_set_pwm_configuration(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        (t_pwm_configuration *) &helicopter_DW.HILInitialize_POModeValues[0],
        (t_pwm_alignment *) &helicopter_DW.HILInitialize_POAlignValues[0],
        (t_pwm_polarity *) &helicopter_DW.HILInitialize_POPolarityVals[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }

      {
        int_T i1;
        real_T *dw_POSortedFreqs = &helicopter_DW.HILInitialize_POSortedFreqs[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POSortedFreqs[i1] = helicopter_P.HILInitialize_POLeading;
        }
      }

      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POTrailing;
        }
      }

      result = hil_set_pwm_deadband(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        &helicopter_DW.HILInitialize_POSortedFreqs[0],
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if ((helicopter_P.HILInitialize_POStart && !is_switching) ||
        (helicopter_P.HILInitialize_POEnter && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POInitial;
        }
      }

      result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
        helicopter_P.HILInitialize_POChannels, 8U,
        &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }

    if (helicopter_P.HILInitialize_POReset) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POWatchdog;
        }
      }

      result = hil_watchdog_set_pwm_expiration_state
        (helicopter_DW.HILInitialize_Card, helicopter_P.HILInitialize_POChannels,
         8U, &helicopter_DW.HILInitialize_POValues[0]);
      if (result < 0) {
        msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
          (_rt_error_message));
        rtmSetErrorStatus(helicopter_M, _rt_error_message);
        return;
      }
    }
  }

  /* Start for S-Function (hil_read_encoder_timebase_block): '<S4>/HIL Read Encoder Timebase' */

  /* S-Function Block: helicopter/Helicopter_interface/HIL Read Encoder Timebase (hil_read_encoder_timebase_block) */
  {
    t_error result;
    result = hil_task_create_encoder_reader(helicopter_DW.HILInitialize_Card,
      helicopter_P.HILReadEncoderTimebase_SamplesI,
      helicopter_P.HILReadEncoderTimebase_Channels, 3,
      &helicopter_DW.HILReadEncoderTimebase_Task);
    if (result >= 0) {
      result = hil_task_set_buffer_overflow_mode
        (helicopter_DW.HILReadEncoderTimebase_Task, (t_buffer_overflow_mode)
         (helicopter_P.HILReadEncoderTimebase_Overflow - 1));
    }

    if (result < 0) {
      msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
        (_rt_error_message));
      rtmSetErrorStatus(helicopter_M, _rt_error_message);
    }
  }

  /* Start for FromFile: '<Root>/From File1' */
  {
    static const real_T tuData[362] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5,
      1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75,
      5.0, 5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0,
      8.25, 8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0,
      11.25, 11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75,
      14.0, 14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5,
      16.75, 17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25,
      19.5, 19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0,
      22.25, 22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75,
      25.0, 25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5,
      27.75, 28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25,
      30.5, 30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0,
      33.25, 33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0, 35.25, 35.5, 35.75,
      36.0, 36.25, 36.5, 36.75, 37.0, 37.25, 37.5, 37.75, 38.0, 38.25, 38.5,
      38.75, 39.0, 39.25, 39.5, 39.75, 40.0, 40.25, 40.5, 40.75, 41.0, 41.25,
      41.5, 41.75, 42.0, 42.25, 42.5, 42.75, 43.0, 43.25, 43.5, 43.75, 44.0,
      44.25, 44.5, 44.75, 45.0, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1415926535897931, 3.1415926535897931, 3.1415926535897931,
      3.1378421413625204, 3.126215553457981, 3.1033093000299292,
      3.0666274151910971, 3.0144539223939932, 2.9456562771172967,
      2.8595077632931347, 2.7555515879645918, 2.6335051104895668,
      2.4931956060314135, 2.3356764181630632, 2.1638071321320393,
      1.9816445083457015, 1.7936953638329192, 1.6043532739672317,
      1.4175586348935518, 1.2366341514123307, 1.0642360893078979,
      0.90237362050429548, 0.752463992384345, 0.61540365895626448,
      0.49164394861605881, 0.38126506776022057, 0.28404528166398896,
      0.19952381140579378, 0.12705690666324332, 0.065867047560932146,
      0.015085493696048315, -0.02621145714322513, -0.058972058358896309,
      -0.084144066795989653, -0.10265455237762368, -0.11539405500671555,
      -0.12320458561296223, -0.12687098419577258, -0.1271151706339386,
      -0.12459285218206262, -0.11989228344262576, -0.11353470886374271,
      -0.10597615331683853, -0.097610262082882149, -0.088771926817195132,
      -0.079741468127765738, -0.070749177792433768, -0.061980053988338141,
      -0.053578590958507061, -0.045653510139081664, -0.038282342842533519,
      -0.031515795131777609, -0.025381843576941208, -0.019889526253890771,
      -0.015032406747749654, -0.010791700215879994, -0.0071390599096503148,
      -0.0040390301287458272, -0.0014511775656598726, 0.00066808242952165833,
      0.0023639447981245796, 0.0036819368031878676, 0.0046668796532671137,
      0.0053620656409600516, 0.0058086267558103422, 0.00604507127679386,
      0.0061069657927335973, 0.00602674134285851, 0.0058336038297452765,
      0.0055535304602059177, 0.0052093356544683951, 0.0048207915786200916,
      0.0044047901574599, 0.0039755350807949581, 0.0035447538995071071,
      0.0031219217987394419, 0.0027144900204046085, 0.0023281131769536632,
      0.001966870848191879, 0.0016334798815881416, 0.0013294947255130716,
      0.0010554939179106561, 0.00081125153552204265, 0.0005958929876586866,
      0.0004080350212395777, 0.00024591019844895097, 0.00010747642322118764,
      -9.4876639860614991E-6, -0.0001073004242228661, -0.00018830696937067036,
      -0.00025481972999207546, -0.00030906862609566236, -0.00035315965222426112,
      -0.00038904077396913781, -0.00041847419794186679, -0.00044301435921594446,
      -0.00046399142872600074, -0.00048250085518836919, -0.00049940048563748584,
      -0.00051531810568492507, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    } ;

    helicopter_DW.FromFile1_PWORK.PrevTimePtr = (void *) &tuData[0];
  }

  /* Start for FromFile: '<Root>/From File2' */
  {
    static const real_T tuData[362] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5,
      1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75,
      5.0, 5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0,
      8.25, 8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0,
      11.25, 11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75,
      14.0, 14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5,
      16.75, 17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25,
      19.5, 19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0,
      22.25, 22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75,
      25.0, 25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5,
      27.75, 28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25,
      30.5, 30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0,
      33.25, 33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0, 35.25, 35.5, 35.75,
      36.0, 36.25, 36.5, 36.75, 37.0, 37.25, 37.5, 37.75, 38.0, 38.25, 38.5,
      38.75, 39.0, 39.25, 39.5, 39.75, 40.0, 40.25, 40.5, 40.75, 41.0, 41.25,
      41.5, 41.75, 42.0, 42.25, 42.5, 42.75, 43.0, 43.25, 43.5, 43.75, 44.0,
      44.25, 44.5, 44.75, 45.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, -0.015002048909120358, -0.046506351617951747,
      -0.091625013712233735, -0.14672753935538879, -0.20869397118836688,
      -0.27519058110685962, -0.34459405529662418, -0.41582470131411842,
      -0.48818590990012684, -0.56123801783251659, -0.63007675147343467,
      -0.687477144124092, -0.72865049514535263, -0.75179657805110822,
      -0.75736835946272663, -0.74717855629470953, -0.7236979339248768,
      -0.68959224841773525, -0.64744987521440545, -0.59963851247980027,
      -0.54824133371232664, -0.49503884136081916, -0.44151552342335543,
      -0.38887914438492693, -0.33808588103278403, -0.28986761897019431,
      -0.24475943640923564, -0.20312621545953882, -0.16518780335709163,
      -0.13104240486267693, -0.10068803374837849, -0.0740419423265379,
      -0.050958010516381533, -0.031242122424987265, -0.014665594331245655,
      -0.000976745752662163, 0.010089273807505753, 0.018802274957747046,
      0.025430298315531744, 0.030234222187616438, 0.033463564935823954,
      0.035353341062747595, 0.036121834757717082, 0.035969161341328541,
      0.035076495216382918, 0.033605852119324285, 0.03170032327770133,
      0.02948466918619249, 0.027066190843023762, 0.024535806219345455,
      0.021969269292201644, 0.019428478024564256, 0.016962826127478129,
      0.014610561224918434, 0.012400119123618249, 0.010351410252343341,
      0.0084770399807269879, 0.00678344947441158, 0.0052719680202524587,
      0.0039397714003173243, 0.002780743950771717, 0.001786244459401298,
      0.00094577808393421817, 0.00024757806375888574, -0.00032089779950050703,
      -0.00077255005245239724, -0.0011202934781574159, -0.0013767792229500563,
      -0.0015541763033933835, -0.0016640056846405843, -0.0017170203066597682,
      -0.0017231247251513955, -0.0016913284030707128, -0.001629727113339297,
      -0.001545507373803811, -0.001444969315047119, -0.0013335638664149234,
      -0.0012159406243002519, -0.001096003230409691, -0.00097696952955443053,
      -0.00086143419145343341, -0.00075143186567644361, -0.00064849929116250413,
      -0.00055373510091105357, -0.00046785634882898387, -0.00039125104094721064,
      -0.00032402618059122415, -0.00026605104248563058, -0.00021699558441434362,
      -0.00017636410451440457, -0.00014352448697950505, -0.00011773369589090982,
      -9.8160645096309631E-5, -8.3908278040224928E-5, -7.4037705849471343E-5,
      -6.7598521796468025E-5, -6.3670480189756819E-5, -6.1421098707827861E-5,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromFile2_PWORK.PrevTimePtr = (void *) &tuData[0];
  }

  /* Start for FromFile: '<Root>/From File3' */
  {
    static const real_T tuData[362] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5,
      1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75,
      5.0, 5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0,
      8.25, 8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0,
      11.25, 11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75,
      14.0, 14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5,
      16.75, 17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25,
      19.5, 19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0,
      22.25, 22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75,
      25.0, 25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5,
      27.75, 28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25,
      30.5, 30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0,
      33.25, 33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0, 35.25, 35.5, 35.75,
      36.0, 36.25, 36.5, 36.75, 37.0, 37.25, 37.5, 37.75, 38.0, 38.25, 38.5,
      38.75, 39.0, 39.25, 39.5, 39.75, 40.0, 40.25, 40.5, 40.75, 41.0, 41.25,
      41.5, 41.75, 42.0, 42.25, 42.5, 42.75, 43.0, 43.25, 43.5, 43.75, 44.0,
      44.25, 44.5, 44.75, 45.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.10602875205917839, 0.22266037932368543, 0.31888147181710869,
      0.3894436063122273, 0.43795507377747944, 0.46997264230443525,
      0.49051724877590763, 0.50343100141519159, 0.5114213858608222,
      0.51630439857211885, 0.48652587826351323, 0.40568405271330132,
      0.29099751996034284, 0.16358767395273954, 0.0393792230255382,
      -0.07201763706864045, -0.16595207111454535, -0.24104595941088458,
      -0.29784619864997697, -0.33791245153839955, -0.3632556297523567,
      -0.3760148966730501, -0.37828236938401461, -0.37201382398820571,
      -0.35898738624611842, -0.34078826058412015, -0.3188074064783078,
      -0.29424770497838693, -0.26813420717929709, -0.2413266356377059,
      -0.21453310199643133, -0.18832439740821078, -0.16314841374427413,
      -0.13934436707403025, -0.11715656960515952, -0.096747553663744268,
      -0.078210399880952464, -0.061580164431879358, -0.046844337696256511,
      -0.033952299197268665, -0.022823761182790836, -0.013356215915137137,
      -0.005431419929886161, 0.0010790373981076629, 0.0063090232417829517,
      0.010393943737850964, 0.013467550087470359, 0.015659396909630252,
      0.017092881257072312, 0.017883792108099983, 0.018139302781589706,
      0.017957342293060674, 0.017426285919466489, 0.016624909947278277,
      0.015622560553401721, 0.014479491853245707, 0.013247333214826185,
      0.011969650877652449, 0.010682573648685856, 0.00941545691343959,
      0.0081915633544408228, 0.0070287425831098149, 0.0059400953487233066,
      0.0049346110843415669, 0.004017770287931266, 0.0031921056279333726,
      0.0024577177221370983, 0.0018127432868783044, 0.0012537748130554416,
      0.00077623212081223691, 0.00037468710146548734, 4.3143699030578908E-5,
      -0.00022472426015884839, -0.00043537438830099084, -0.00059523295300212276,
      -0.00071056460090856977, -0.00078737116203608792, -0.000831316151646222,
      -0.00084767169256505515, -0.00084128473532729675, -0.00081655964348092969,
      -0.0007774544255545468, -0.00072748812372303018, -0.00066975710344496786,
      -0.00060695822001211042, -0.00054141705814136909, -0.00047511963763280374,
      -0.00040974613344357813, -0.00034670524158675331, -0.0002871677811777662,
      -0.00023209787386790914, -0.00018227946064952594, -0.00013833484694514286,
      -0.00010073028636858883, -6.9761433765025416E-5, -4.55096929304899E-5,
      -2.7761897181122208E-5, -1.589776922328312E-5, -8.7892277264023377E-6, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromFile3_PWORK.PrevTimePtr = (void *) &tuData[0];
  }

  /* Start for FromFile: '<Root>/From File4' */
  {
    static const real_T tuData[362] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5,
      1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75,
      5.0, 5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0,
      8.25, 8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0,
      11.25, 11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75,
      14.0, 14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5,
      16.75, 17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25,
      19.5, 19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0,
      22.25, 22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75,
      25.0, 25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5,
      27.75, 28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25,
      30.5, 30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0,
      33.25, 33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0, 35.25, 35.5, 35.75,
      36.0, 36.25, 36.5, 36.75, 37.0, 37.25, 37.5, 37.75, 38.0, 38.25, 38.5,
      38.75, 39.0, 39.25, 39.5, 39.75, 40.0, 40.25, 40.5, 40.75, 41.0, 41.25,
      41.5, 41.75, 42.0, 42.25, 42.5, 42.75, 43.0, 43.25, 43.5, 43.75, 44.0,
      44.25, 44.5, 44.75, 45.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.42411500823577752, 0.46652650905898135, 0.38488436997369557,
      0.282248537980475, 0.19404586986100877, 0.1280702741078199,
      0.082178425885670051, 0.051655010556732252, 0.03196153778204066,
      0.019532050844799698, -0.11911408123479457, -0.32336730220114246,
      -0.45874613101209166, -0.50963938403062892, -0.49683380370898927,
      -0.4455874403768636, -0.37573773618373968, -0.30037555318544984,
      -0.22720095695643991, -0.16026501155373846, -0.10137271285585661,
      -0.0510370676827854, -0.009069890843855434, 0.02507418158325031,
      0.052105750968373575, 0.072796502648024689, 0.0879234164232877,
      0.098238805999725912, 0.10445399119640612, 0.10723028616641624,
      0.10717413456515247, 0.10483481835293652, 0.10070393465580056,
      0.095216186681029324, 0.088751189875535055, 0.081636063765713016,
      0.07414861513121726, 0.066520941796340632, 0.058943306942537396,
      0.051568153995994938, 0.044514152057951929, 0.037870181070652646,
      0.03169918394103919, 0.026041829312008134, 0.020919943374731471,
      0.016339681984299206, 0.012294425398502614, 0.0087673872886619691,
      0.00573393738978831, 0.0031636434041285135, 0.0010220426939743016,
      -0.00072784195410247022, -0.0021242254943651933, -0.0032055038887426638,
      -0.0040093975754979068, -0.0045722748006174936, -0.0049286345536726974,
      -0.00511072934869118, -0.0051483089158634995, -0.0050684669409836059,
      -0.004895574235994773, -0.0046512830853248591, -0.0043545889375478878,
      -0.004021937057529788, -0.00366736318564518, -0.0033026586399967309,
      -0.0029375516231911905, -0.0025798977410417816, -0.0022358738952995495,
      -0.0019101707689818405, -0.0016061800773976235, -0.0013261736097507206,
      -0.0010714718367702187, -0.00084260051258225267, -0.00063943425881981706,
      -0.00046132659164182046, -0.00030722624452786032, -0.00017577995845904774,
      -6.5422163695599391E-5, 2.5547828929927606E-5, 9.8900367362535186E-5,
      0.00015642087168245725, 0.00019986520730095467, 0.00023092408108628114,
      0.000251195533705145, 0.00026216464745562037, 0.00026518968200659994,
      0.00026149401672903783, 0.00025216356739933749, 0.00023814984160850129,
      0.00022027962921269709, 0.00019927365284861621, 0.00017577845479389086,
      0.00015041824228527727, 0.00012387541039627397, 9.700696332354413E-5,
      7.0991182986859021E-5, 4.7456511824610093E-5, 2.8434165984423162E-5,
      1.5720609669301645E-5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    } ;

    helicopter_DW.FromFile4_PWORK.PrevTimePtr = (void *) &tuData[0];
  }

  /* Start for FromFile: '<Root>/From File5' */
  {
    static const real_T tuData[362] = { 0.0, 0.25, 0.5, 0.75, 1.0, 1.25, 1.5,
      1.75, 2.0, 2.25, 2.5, 2.75, 3.0, 3.25, 3.5, 3.75, 4.0, 4.25, 4.5, 4.75,
      5.0, 5.25, 5.5, 5.75, 6.0, 6.25, 6.5, 6.75, 7.0, 7.25, 7.5, 7.75, 8.0,
      8.25, 8.5, 8.75, 9.0, 9.25, 9.5, 9.75, 10.0, 10.25, 10.5, 10.75, 11.0,
      11.25, 11.5, 11.75, 12.0, 12.25, 12.5, 12.75, 13.0, 13.25, 13.5, 13.75,
      14.0, 14.25, 14.5, 14.75, 15.0, 15.25, 15.5, 15.75, 16.0, 16.25, 16.5,
      16.75, 17.0, 17.25, 17.5, 17.75, 18.0, 18.25, 18.5, 18.75, 19.0, 19.25,
      19.5, 19.75, 20.0, 20.25, 20.5, 20.75, 21.0, 21.25, 21.5, 21.75, 22.0,
      22.25, 22.5, 22.75, 23.0, 23.25, 23.5, 23.75, 24.0, 24.25, 24.5, 24.75,
      25.0, 25.25, 25.5, 25.75, 26.0, 26.25, 26.5, 26.75, 27.0, 27.25, 27.5,
      27.75, 28.0, 28.25, 28.5, 28.75, 29.0, 29.25, 29.5, 29.75, 30.0, 30.25,
      30.5, 30.75, 31.0, 31.25, 31.5, 31.75, 32.0, 32.25, 32.5, 32.75, 33.0,
      33.25, 33.5, 33.75, 34.0, 34.25, 34.5, 34.75, 35.0, 35.25, 35.5, 35.75,
      36.0, 36.25, 36.5, 36.75, 37.0, 37.25, 37.5, 37.75, 38.0, 38.25, 38.5,
      38.75, 39.0, 39.25, 39.5, 39.75, 40.0, 40.25, 40.5, 40.75, 41.0, 41.25,
      41.5, 41.75, 42.0, 42.25, 42.5, 42.75, 43.0, 43.25, 43.5, 43.75, 44.0,
      44.25, 44.5, 44.75, 45.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.52359877559971546, 0.52359877559925916, 0.52359877559943679,
      0.5235987755991246, 0.52359877559863122, 0.52359877559852175,
      0.52359877559869761, 0.52359877559886614, 0.52359877559913393,
      0.52359877557182466, 0.36195560028185614, 0.1317909490938457,
      -0.03990548073864697, -0.1668650471993306, -0.25945910387393162,
      -0.32518277049890759, -0.36948373024106435, -0.39646427850937488,
      -0.4093636780748357, -0.41085449750724734, -0.4032118921073326,
      -0.38840602733740182, -0.36815215330245665, -0.34393937733030433,
      -0.31704985974200617, -0.28857440725838512, -0.25942718111223034,
      -0.23036052680960273, -0.20198010944572692, -0.17476019010588134,
      -0.14905876773675136, -0.12513230860514751, -0.10314982690944607,
      -0.083206134179320879, -0.065334128303059236, -0.049516040187470911,
      -0.035693595834047631, -0.02377708417834179, -0.013653347087294931,
      -0.00519272825693784, 0.0017449668201828805, 0.00730543643831616,
      0.011635702809730586, 0.01488071114396794, 0.017180630000365626,
      0.01866877523366528, 0.01947008151345031, 0.019700048365989042,
      0.019464091644142534, 0.018857236002388089, 0.017964089100958258,
      0.016859043684011144, 0.015606659202620055, 0.014262180145758419,
      0.012872153590292368, 0.01147511359780462, 0.010102304906962734,
      0.0087784228491378791, 0.0075223505222378195, 0.0063478779771605209,
      0.0052643914981962414, 0.0042775239977302926, 0.0033897601087450324,
      0.0026009917636379276, 0.0019090219167622324, 0.0013100156257890294,
      0.00079889898029428252, 0.00036970738260466707, 1.5885473756749313E-5,
      -0.0002694584161383462, -0.00049334000236445075, -0.00066272541252188066,
      -0.000784392498267561, -0.00086482337367654871, -0.00091012451639094216,
      -0.00092597088922097281, -0.00091757071269082946, -0.00088964772929034375,
      -0.00084643803590223055, -0.00079169881174645518, -0.00072872652652811176,
      -0.00066038246993516125, -0.00058912369267216658, -0.00051703768434530062,
      -0.00044587932666062535, -0.000377108839112017, -0.00031192955669590017,
      -0.000251324405606157, -0.00019608980337160187, -0.00014686528789631392,
      -0.00010415629704885188, -6.8345941733327642E-5, -3.9689121361763391E-5,
      -1.8278970742580114E-5, -3.9723948663805686E-6, 3.7381362198996015E-6,
      5.9058337210649015E-6, 4.314249700820838E-6, 1.4831828240025047E-6, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } ;

    helicopter_DW.FromFile5_PWORK.PrevTimePtr = (void *) &tuData[0];
  }

  /* Start for ToFile: '<Root>/To File' */
  {
    FILE *fp = (NULL);
    char fileName[509] = "dag3_q_001_r_1.mat";
    if ((fp = fopen(fileName, "wb")) == (NULL)) {
      rtmSetErrorStatus(helicopter_M,
                        "Error creating .mat file dag3_q_001_r_1.mat");
      return;
    }

    if (rt_WriteMat4FileHeader(fp, 6 + 1, 0, "ans")) {
      rtmSetErrorStatus(helicopter_M,
                        "Error writing mat file header to file dag3_q_001_r_1.mat");
      return;
    }

    helicopter_DW.ToFile_IWORK.Count = 0;
    helicopter_DW.ToFile_IWORK.Decimation = -1;
    helicopter_DW.ToFile_PWORK.FilePtr = fp;
  }

  /* Start for If: '<S3>/If' */
  helicopter_DW.If_ActiveSubsystem = -1;

  /* InitializeConditions for Integrator: '<S3>/Integrator' */
  helicopter_X.Integrator_CSTATE = helicopter_P.Integrator_IC;

  /* InitializeConditions for TransferFcn: '<S4>/Travel: Transfer Fcn' */
  helicopter_X.TravelTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Pitch: Transfer Fcn' */
  helicopter_X.PitchTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S4>/Elevation: Transfer Fcn' */
  helicopter_X.ElevationTransferFcn_CSTATE = 0.0;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
  helicopter_DW.TimeStampA = (rtInf);
  helicopter_DW.TimeStampB = (rtInf);
}

/* Model terminate function */
void helicopter_terminate(void)
{
  /* Terminate for S-Function (hil_initialize_block): '<Root>/HIL Initialize' */

  /* S-Function Block: helicopter/HIL Initialize (hil_initialize_block) */
  {
    t_boolean is_switching;
    t_int result;
    t_uint32 num_final_analog_outputs = 0;
    t_uint32 num_final_pwm_outputs = 0;
    hil_task_stop_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_stop_all(helicopter_DW.HILInitialize_Card);
    is_switching = false;
    if ((helicopter_P.HILInitialize_AOTerminate && !is_switching) ||
        (helicopter_P.HILInitialize_AOExit && is_switching)) {
      {
        int_T i1;
        real_T *dw_AOVoltages = &helicopter_DW.HILInitialize_AOVoltages[0];
        for (i1=0; i1 < 8; i1++) {
          dw_AOVoltages[i1] = helicopter_P.HILInitialize_AOFinal;
        }
      }

      num_final_analog_outputs = 8U;
    } else {
      num_final_analog_outputs = 0;
    }

    if ((helicopter_P.HILInitialize_POTerminate && !is_switching) ||
        (helicopter_P.HILInitialize_POExit && is_switching)) {
      {
        int_T i1;
        real_T *dw_POValues = &helicopter_DW.HILInitialize_POValues[0];
        for (i1=0; i1 < 8; i1++) {
          dw_POValues[i1] = helicopter_P.HILInitialize_POFinal;
        }
      }

      num_final_pwm_outputs = 8U;
    } else {
      num_final_pwm_outputs = 0;
    }

    if (0
        || num_final_analog_outputs > 0
        || num_final_pwm_outputs > 0
        ) {
      /* Attempt to write the final outputs atomically (due to firmware issue in old Q2-USB). Otherwise write channels individually */
      result = hil_write(helicopter_DW.HILInitialize_Card
                         , helicopter_P.HILInitialize_AOChannels,
                         num_final_analog_outputs
                         , helicopter_P.HILInitialize_POChannels,
                         num_final_pwm_outputs
                         , NULL, 0
                         , NULL, 0
                         , &helicopter_DW.HILInitialize_AOVoltages[0]
                         , &helicopter_DW.HILInitialize_POValues[0]
                         , (t_boolean *) NULL
                         , NULL
                         );
      if (result == -QERR_HIL_WRITE_NOT_SUPPORTED) {
        t_error local_result;
        result = 0;

        /* The hil_write operation is not supported by this card. Write final outputs for each channel type */
        if (num_final_analog_outputs > 0) {
          local_result = hil_write_analog(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_AOChannels, num_final_analog_outputs,
            &helicopter_DW.HILInitialize_AOVoltages[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (num_final_pwm_outputs > 0) {
          local_result = hil_write_pwm(helicopter_DW.HILInitialize_Card,
            helicopter_P.HILInitialize_POChannels, num_final_pwm_outputs,
            &helicopter_DW.HILInitialize_POValues[0]);
          if (local_result < 0) {
            result = local_result;
          }
        }

        if (result < 0) {
          msg_get_error_messageA(NULL, result, _rt_error_message, sizeof
            (_rt_error_message));
          rtmSetErrorStatus(helicopter_M, _rt_error_message);
        }
      }
    }

    hil_task_delete_all(helicopter_DW.HILInitialize_Card);
    hil_monitor_delete_all(helicopter_DW.HILInitialize_Card);
    hil_close(helicopter_DW.HILInitialize_Card);
    helicopter_DW.HILInitialize_Card = NULL;
  }

  /* Terminate for ToFile: '<Root>/To File' */
  {
    FILE *fp = (FILE *) helicopter_DW.ToFile_PWORK.FilePtr;
    if (fp != (NULL)) {
      char fileName[509] = "dag3_q_001_r_1.mat";
      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file dag3_q_001_r_1.mat");
        return;
      }

      if ((fp = fopen(fileName, "r+b")) == (NULL)) {
        rtmSetErrorStatus(helicopter_M,
                          "Error reopening MAT-file dag3_q_001_r_1.mat");
        return;
      }

      if (rt_WriteMat4FileHeader(fp, 6 + 1, helicopter_DW.ToFile_IWORK.Count,
           "ans")) {
        rtmSetErrorStatus(helicopter_M,
                          "Error writing header for ans to MAT-file dag3_q_001_r_1.mat");
      }

      if (fclose(fp) == EOF) {
        rtmSetErrorStatus(helicopter_M,
                          "Error closing MAT-file dag3_q_001_r_1.mat");
        return;
      }

      helicopter_DW.ToFile_PWORK.FilePtr = (NULL);
    }
  }
}

/*========================================================================*
 * Start of Classic call interface                                        *
 *========================================================================*/

/* Solver interface called by GRT_Main */
#ifndef USE_GENERATED_SOLVER

void rt_ODECreateIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEDestroyIntegrationData(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

void rt_ODEUpdateContinuousStates(RTWSolverInfo *si)
{
  UNUSED_PARAMETER(si);
  return;
}                                      /* do nothing */

#endif

void MdlOutputs(int_T tid)
{
  helicopter_output();
  UNUSED_PARAMETER(tid);
}

void MdlUpdate(int_T tid)
{
  helicopter_update();
  UNUSED_PARAMETER(tid);
}

void MdlInitializeSizes(void)
{
}

void MdlInitializeSampleTimes(void)
{
}

void MdlInitialize(void)
{
}

void MdlStart(void)
{
  helicopter_initialize();
}

void MdlTerminate(void)
{
  helicopter_terminate();
}

/* Registration function */
RT_MODEL_helicopter_T *helicopter(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* non-finite (run-time) assignments */
  helicopter_P.Integrator_UpperSat = rtInf;
  helicopter_P.Integrator_LowerSat = rtMinusInf;

  /* initialize real-time model */
  (void) memset((void *)helicopter_M, 0,
                sizeof(RT_MODEL_helicopter_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&helicopter_M->solverInfo,
                          &helicopter_M->Timing.simTimeStep);
    rtsiSetTPtr(&helicopter_M->solverInfo, &rtmGetTPtr(helicopter_M));
    rtsiSetStepSizePtr(&helicopter_M->solverInfo,
                       &helicopter_M->Timing.stepSize0);
    rtsiSetdXPtr(&helicopter_M->solverInfo, &helicopter_M->derivs);
    rtsiSetContStatesPtr(&helicopter_M->solverInfo, (real_T **)
                         &helicopter_M->contStates);
    rtsiSetNumContStatesPtr(&helicopter_M->solverInfo,
      &helicopter_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&helicopter_M->solverInfo,
      &helicopter_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&helicopter_M->solverInfo,
      &helicopter_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&helicopter_M->solverInfo,
      &helicopter_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&helicopter_M->solverInfo, (&rtmGetErrorStatus
      (helicopter_M)));
    rtsiSetRTModelPtr(&helicopter_M->solverInfo, helicopter_M);
  }

  rtsiSetSimTimeStep(&helicopter_M->solverInfo, MAJOR_TIME_STEP);
  helicopter_M->intgData.f[0] = helicopter_M->odeF[0];
  helicopter_M->contStates = ((real_T *) &helicopter_X);
  rtsiSetSolverData(&helicopter_M->solverInfo, (void *)&helicopter_M->intgData);
  rtsiSetSolverName(&helicopter_M->solverInfo,"ode1");

  /* Initialize timing info */
  {
    int_T *mdlTsMap = helicopter_M->Timing.sampleTimeTaskIDArray;
    mdlTsMap[0] = 0;
    mdlTsMap[1] = 1;
    helicopter_M->Timing.sampleTimeTaskIDPtr = (&mdlTsMap[0]);
    helicopter_M->Timing.sampleTimes = (&helicopter_M->Timing.sampleTimesArray[0]);
    helicopter_M->Timing.offsetTimes = (&helicopter_M->Timing.offsetTimesArray[0]);

    /* task periods */
    helicopter_M->Timing.sampleTimes[0] = (0.0);
    helicopter_M->Timing.sampleTimes[1] = (0.002);

    /* task offsets */
    helicopter_M->Timing.offsetTimes[0] = (0.0);
    helicopter_M->Timing.offsetTimes[1] = (0.0);
  }

  rtmSetTPtr(helicopter_M, &helicopter_M->Timing.tArray[0]);

  {
    int_T *mdlSampleHits = helicopter_M->Timing.sampleHitArray;
    mdlSampleHits[0] = 1;
    mdlSampleHits[1] = 1;
    helicopter_M->Timing.sampleHits = (&mdlSampleHits[0]);
  }

  rtmSetTFinal(helicopter_M, -1);
  helicopter_M->Timing.stepSize0 = 0.002;
  helicopter_M->Timing.stepSize1 = 0.002;

  /* External mode info */
  helicopter_M->Sizes.checksums[0] = (327668345U);
  helicopter_M->Sizes.checksums[1] = (434364533U);
  helicopter_M->Sizes.checksums[2] = (4267966282U);
  helicopter_M->Sizes.checksums[3] = (2297045019U);

  {
    static const sysRanDType rtAlwaysEnabled = SUBSYS_RAN_BC_ENABLE;
    static RTWExtModeInfo rt_ExtModeInfo;
    static const sysRanDType *systemRan[2];
    helicopter_M->extModeInfo = (&rt_ExtModeInfo);
    rteiSetSubSystemActiveVectorAddresses(&rt_ExtModeInfo, systemRan);
    systemRan[0] = &rtAlwaysEnabled;
    systemRan[1] = (sysRanDType *)&helicopter_DW.IfActionSubsystem_SubsysRanBC;
    rteiSetModelMappingInfoPtr(helicopter_M->extModeInfo,
      &helicopter_M->SpecialInfo.mappingInfo);
    rteiSetChecksumsPtr(helicopter_M->extModeInfo, helicopter_M->Sizes.checksums);
    rteiSetTPtr(helicopter_M->extModeInfo, rtmGetTPtr(helicopter_M));
  }

  helicopter_M->solverInfoPtr = (&helicopter_M->solverInfo);
  helicopter_M->Timing.stepSize = (0.002);
  rtsiSetFixedStepSize(&helicopter_M->solverInfo, 0.002);
  rtsiSetSolverMode(&helicopter_M->solverInfo, SOLVER_MODE_SINGLETASKING);

  /* block I/O */
  helicopter_M->blockIO = ((void *) &helicopter_B);

  {
    helicopter_B.TravelCounttorad = 0.0;
    helicopter_B.Gain = 0.0;
    helicopter_B.Gain_d = 0.0;
    helicopter_B.PitchCounttorad = 0.0;
    helicopter_B.Gain_i = 0.0;
    helicopter_B.Gain_b = 0.0;
    helicopter_B.ElevationCounttorad = 0.0;
    helicopter_B.Gain_e = 0.0;
    helicopter_B.Sum = 0.0;
    helicopter_B.Gain_dg = 0.0;
    helicopter_B.Sum2 = 0.0;
    helicopter_B.Sum1 = 0.0;
    helicopter_B.Gain_l = 0.0;
    helicopter_B.BackmotorSaturation = 0.0;
    helicopter_B.FrontmotorSaturation = 0.0;
    helicopter_B.In1 = 0.0;
  }

  /* parameters */
  helicopter_M->defaultParam = ((real_T *)&helicopter_P);

  /* states (continuous) */
  {
    real_T *x = (real_T *) &helicopter_X;
    helicopter_M->contStates = (x);
    (void) memset((void *)&helicopter_X, 0,
                  sizeof(X_helicopter_T));
  }

  /* states (dwork) */
  helicopter_M->dwork = ((void *) &helicopter_DW);
  (void) memset((void *)&helicopter_DW, 0,
                sizeof(DW_helicopter_T));

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AIMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMinimums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOMaximums[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_AOVoltages[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_FilterFrequency[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POSortedFreqs[i] = 0.0;
    }
  }

  {
    int32_T i;
    for (i = 0; i < 8; i++) {
      helicopter_DW.HILInitialize_POValues[i] = 0.0;
    }
  }

  helicopter_DW.TimeStampA = 0.0;
  helicopter_DW.LastUAtTimeA = 0.0;
  helicopter_DW.TimeStampB = 0.0;
  helicopter_DW.LastUAtTimeB = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[0] = 0.0;
  helicopter_DW.HILWriteAnalog_Buffer[1] = 0.0;

  /* data type transition information */
  {
    static DataTypeTransInfo dtInfo;
    (void) memset((char_T *) &dtInfo, 0,
                  sizeof(dtInfo));
    helicopter_M->SpecialInfo.mappingInfo = (&dtInfo);
    dtInfo.numDataTypes = 16;
    dtInfo.dataTypeSizes = &rtDataTypeSizes[0];
    dtInfo.dataTypeNames = &rtDataTypeNames[0];

    /* Block I/O transition table */
    dtInfo.BTransTable = &rtBTransTable;

    /* Parameters transition table */
    dtInfo.PTransTable = &rtPTransTable;
  }

  /* Initialize Sizes */
  helicopter_M->Sizes.numContStates = (4);/* Number of continuous states */
  helicopter_M->Sizes.numPeriodicContStates = (0);
                                      /* Number of periodic continuous states */
  helicopter_M->Sizes.numY = (0);      /* Number of model outputs */
  helicopter_M->Sizes.numU = (0);      /* Number of model inputs */
  helicopter_M->Sizes.sysDirFeedThru = (0);/* The model is not direct feedthrough */
  helicopter_M->Sizes.numSampTimes = (2);/* Number of sample times */
  helicopter_M->Sizes.numBlocks = (71);/* Number of blocks */
  helicopter_M->Sizes.numBlockIO = (16);/* Number of block outputs */
  helicopter_M->Sizes.numBlockPrms = (150);/* Sum of parameter "widths" */
  return helicopter_M;
}

/*========================================================================*
 * End of Classic call interface                                          *
 *========================================================================*/
