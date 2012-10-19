function [status, varargout] = IDACalcICB(tout,icmeth)
%IDACalcICB computes consistent initial conditions for the backward phase.
%
%   Usage: STATUS = IDACalcICB ( TOUTB, ICMETHB )
%          [STATUS, YY0B, YP0B] = IDACalcIC ( TOUTB, ICMETHB )
%
%  See also: IDASetOptions, IDAInitB, IDAReInitB

% Radu Serban <radu@llnl.gov>
% Copyright (c) 2005, The Regents of the University of California.
% $Revision: 1.4 $Date: 2011/05/26 00:05:36 $

mode = 26;

if nargout == 1
  status = idm(mode, tout, icmeth);
elseif nargout == 3
  [status, yy, yp] = idm(mode, tout, icmeth);
  varargout(1) = {yy};
  varargout(2) = {yp};
else
  disp('IDACalcICB:: wrong number of output arguments');
end
